//
// B1BaseSessionManager.cpp
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Network.h"
#include "B1BaseSessionManager.h"
#include "B1BaseSession.h"
#include "B1BaseSocket.h"
#include "B1ServerSocket.h"

using namespace BnD;

B1BaseSessionManager::B1BaseSessionManager(uint32 workingInerval)
    : _workingInerval(workingInerval)
    , _sessionsLock()
    , _sessions()
    , _reservedRemoveSessions()
    , _releaseWaitingSessions()
{
}

B1BaseSessionManager::~B1BaseSessionManager()
{
}

void B1BaseSessionManager::implLooperFunc()
{
    std::map<B1BaseSocket*, SessionData> sessions;
    std::list<SessionData> removeSessions;
    {
        B1AutoLock al(_sessionsLock);
        sessions = _sessions;
        removeSessions.swap(_reservedRemoveSessions);
    }
    for (const auto& sessionsPair : sessions) {
        if (sessionsPair.second._session->isConnected()) {
            sessionsPair.second._session->processConnected();
        }
        else if (sessionsPair.second._session->isDisconnecting()) {
            sessionsPair.second._session->setDisconnectedIfSessionClosed();
        }
        else if (sessionsPair.second._session->isDisconnected()) {
            sessionsPair.second._session->processDisconnected();
        }
    }
    for (const auto& removeSession : removeSessions) {
        int32 handleID = removeSession._handleID;
        auto asioSocket = removeSession._baseSocket->asioSocket();
        if (asioSocket && dynamic_cast<B1ServerSocket*>(removeSession._baseSocket.get())) {
            //  server session: io handlers of this session may still be queued(io thread is shared by all sessions).
            //  cleanup and destroy on io thread.
            auto executor = asioSocket->get_executor();
            boost::asio::post(executor, [removeSession, executor]() {
                removeSession._session->cleanupSession();               //  no more async operation after cleanup.
                boost::asio::post(executor, [removeSession]() {});      //  destroyed after handlers of operations started before cleanup.
            });
        }
        else {
            //  client session: destroy on this thread when no other reference remains.
            //  (if the last reference is released on its own io thread, ~B1ClientSocket() joins itself)
            removeSession._session->cleanupSession();
            _releaseWaitingSessions.push_back(removeSession);
        }
        onSessionRemoved(handleID);
    }
    {
        B1AutoLock al(_sessionsLock);
        for (const auto& removeSession : removeSessions) {
            _handleIDMap.erase(removeSession._handleID);
        }
    }
    sessions.clear();
    removeSessions.clear();
    for (auto itr = _releaseWaitingSessions.begin(); itr != _releaseWaitingSessions.end();) {
        if (itr->_session.use_count() < 2 && itr->_baseSocket.use_count() < 2) {
            itr = _releaseWaitingSessions.erase(itr);
        }
        else {
            ++itr;
        }
    }
    B1Thread::sleep(_workingInerval);
}

int32 B1BaseSessionManager::addSession(std::shared_ptr<B1BaseSocket> baseSocket, std::shared_ptr<B1BaseSession> session, int32 requestedHandleID)
{
    if (nullptr == session) {
        assert(false);
        return -1;
    }
    B1AutoLock al(_sessionsLock);
    if (_sessions.find(baseSocket.get()) != _sessions.end()) {
        B1LOG("session already added: peerAddress[%s]", baseSocket->peerAddress().cString());
        baseSocket->close();
        assert(false);
        return -1;
    }
    const int32 newHandleID = requestedHandleID < 0 ? _handleIDMap.empty() ? 0 : _handleIDMap.rbegin()->first + 1 : requestedHandleID;
    if (_handleIDMap.find(newHandleID) != _handleIDMap.end()) {
        B1LOG("session handle_id already added: peerAddress[%s], newHandleID[%d], requestedHandleID[%d]",
              baseSocket->peerAddress().cString(), newHandleID, requestedHandleID);
        baseSocket->close();
        assert(false);
        return -1;
    }
    _handleIDMap.insert(std::make_pair(newHandleID, baseSocket.get()));
    _sessions.insert(std::make_pair(baseSocket.get(), SessionData(newHandleID, session, baseSocket)));
    onSessionAdded(newHandleID, session);
    return newHandleID;
}

bool B1BaseSessionManager::modifySession(B1BaseSocket* baseSocket, std::shared_ptr<B1BaseSession> session)
{
    if (baseSocket == nullptr || session == nullptr) {
        assert(false);
        return false;
    }
    B1AutoLock al(_sessionsLock);
    auto itr = _sessions.find(baseSocket);
    if (itr == _sessions.end()) {
        B1LOG("no session found to modify: peerAddress[%s]", baseSocket->peerAddress().cString());
        assert(false);
        return false;
    }
    session->setSessionHandleID(itr->second._session->sessionHandleID());
    itr->second._session = session;
    B1LOG("session modified: sessionHandleID[%d]", itr->second._session->sessionHandleID());
    onSessionModified(itr->second._handleID, session);
    return true;
}

void B1BaseSessionManager::reserveRemoveSession(B1BaseSocket* baseSocket)
{
    if (isAlive() != true) {
        B1LOG("reserveRemoveSession requested. but manager is not alived");
        return;
    }
    B1AutoLock al(_sessionsLock);
    auto itr = _sessions.find(baseSocket);
    if (itr == _sessions.end()) {
        B1LOG("session already removed: peerAddress[%s]", baseSocket->peerAddress().cString());
        return;
    }
    _reservedRemoveSessions.push_back(itr->second);
    _sessions.erase(itr);
}

bool B1BaseSessionManager::empty()
{
    B1AutoLock al(_sessionsLock);
    return _sessions.empty();
}

bool B1BaseSessionManager::startup()
{
    return run();
}

void B1BaseSessionManager::shutdown()
{
    quit();
    join();
    disconnectAllSessions();
    bool timedout = true;
    const int32 sleepTime = 1000;   //  millisecond.
    const int32 trys = 60;          //  1 min.
    for (int32 i = 0; i < trys; ++i) {
        {
            B1AutoLock al(_sessionsLock);
            for (auto itr = _sessions.cbegin(); itr != _sessions.end();) {
                if (itr->second._session->isDisconnecting()) {
                    itr->second._session->setDisconnectedIfSessionClosed();
                }
                else if (itr->second._session->isDisconnected()) {
                    _reservedRemoveSessions.push_back(itr->second); //  cleanupReservedRemoveSessions() after io stopped.
                    itr = _sessions.erase(itr);
                }
                else {
                    B1LOG("waiting disconnection: sessionHandleID[%d]", itr->second._session->sessionHandleID());
                    ++itr;
                }
            }
            if (_sessions.empty()) {
                timedout = false;
                break;
            }
        }
        B1Thread::sleep(sleepTime);
    }
    if (timedout) {
        B1LOG("session Disconnect timed out! -> remaining sessions are reserved to remove: count[%u]", static_cast<uint32>(_sessions.size()));
        assert(false);
        B1AutoLock al(_sessionsLock);
        for (const auto& sessionsPair : _sessions) {    //  io handlers may still be running. cleanupReservedRemoveSessions() after io stopped.
            _reservedRemoveSessions.push_back(sessionsPair.second);
        }
        _sessions.clear();
    }
    onShuttingDown();
}

void B1BaseSessionManager::cleanupReservedRemoveSessions()
{
    std::list<SessionData> removeSessions;
    std::list<SessionData> releaseWaitingSessions;
    {
        B1AutoLock al(_sessionsLock);
        removeSessions.swap(_reservedRemoveSessions);
        releaseWaitingSessions.swap(_releaseWaitingSessions);
        for (const auto& removeSession : removeSessions) {
            _handleIDMap.erase(removeSession._handleID);
        }
    }
    for (const auto& removeSession : removeSessions) {
        removeSession._session->cleanupSession();
    }
}

void B1BaseSessionManager::disconnectAllSessions()
{
    B1AutoLock al(_sessionsLock);
    for (const auto& sessionsPair : _sessions) {
        sessionsPair.second._session->disconnect();
    }
}

int32 B1BaseSessionManager::getHandleID(B1BaseSocket* baseSocket) const
{
    B1AutoLock al(_sessionsLock);
    auto itr = _sessions.find(baseSocket);
    return itr != _sessions.end() ? itr->second._handleID : -1;
}

uint32 B1BaseSessionManager::sessionCount() const
{
    B1AutoLock al(_sessionsLock);
    return static_cast<uint32>(_sessions.size());
}

auto B1BaseSessionManager::getAllSessions() const -> std::map<B1BaseSocket*, B1BaseSessionManager::SessionData>
{
    B1AutoLock al(_sessionsLock);
    return _sessions;
}

auto B1BaseSessionManager::getBaseSession(B1BaseSocket* baseSocket) const -> std::shared_ptr<B1BaseSession>
{
    B1AutoLock al(_sessionsLock);
    auto itr = _sessions.find(baseSocket);
    return itr != _sessions.end() ? itr->second._session : std::shared_ptr<B1BaseSession>();
}

auto B1BaseSessionManager::getBaseSessionByHandleID(int32 handleID) const -> std::shared_ptr<B1BaseSession>
{
    B1BaseSocket* baseSocket = nullptr;
    B1AutoLock al(_sessionsLock);
    {
        auto itr = _handleIDMap.find(handleID);
        if (itr != _handleIDMap.end()) {
            baseSocket = itr->second;
        }
    }
    auto itr = _sessions.find(baseSocket);
    return itr != _sessions.end() ? itr->second._session : std::shared_ptr<B1BaseSession>();
}
