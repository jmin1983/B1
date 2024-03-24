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

using namespace BnD;

B1BaseSessionManager::B1BaseSessionManager(uint32 workingInerval)
    : _workingInerval(workingInerval)
    , _sessionsLock()
    , _sessions()
    , _reservedRemoveSessions()
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
        else if (sessionsPair.second._session->isDisconnected()) {
            sessionsPair.second._session->processDisconnected();
        }
    }
    for (const auto& removeSession : removeSessions) {
        int32 handleID = removeSession._handleID;
        removeSession._baseSocket->close();
        removeSession._session->finalize();
        onSessionRemoved(handleID);
    }
    B1Thread::sleep(_workingInerval);
}

int32 B1BaseSessionManager::addSession(std::shared_ptr<B1BaseSocket> baseSocket, std::shared_ptr<B1BaseSession> session, int32 requestedHandleID)
{
    if (socket == NULL || session == NULL) {
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
    if (baseSocket == NULL || session == NULL) {
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
    itr->second._session->finalize();
    itr->second._session = session;
    onSessionModified(itr->second._handleID, session);
    return true;
}

void B1BaseSessionManager::reserveRemoveSession(B1BaseSocket* baseSocket)
{
    B1AutoLock al(_sessionsLock);
    auto itr = _sessions.find(baseSocket);
    if (itr == _sessions.end()) {
        B1LOG("session already removed: peerAddress[%s]", baseSocket->peerAddress().cString());
        return;
    }
    _reservedRemoveSessions.push_back(itr->second);
    _handleIDMap.erase(itr->second._handleID);
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
    const int32 sleepTime = 100;    //  millisecond
    const int32 trys = 10 * 60 * 2; //  2min
    for (int32 i = 0; i < trys; ++i) {
        {
            B1AutoLock al(_sessionsLock);
            for (auto itr = _sessions.cbegin(); itr != _sessions.end();) {
                if (itr->second._session->isDisconnected()) {
                    _reservedRemoveSessions.push_back(itr->second);
                    _handleIDMap.erase(itr->second._handleID);
                    itr = _sessions.erase(itr);
                }
                else {
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
        B1LOG("session Disconnect timed out!");
        assert(false);
        _handleIDMap.clear();
        _sessions.clear();
    }
    for (const auto& removeSession : _reservedRemoveSessions) {
        removeSession._session->finalize();
    }
    onShuttingDown();
    _reservedRemoveSessions.clear();
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

std::map<B1BaseSocket*, B1BaseSessionManager::SessionData> B1BaseSessionManager::getAllSessions() const
{
    B1AutoLock al(_sessionsLock);
    return _sessions;
}

std::shared_ptr<B1BaseSession> B1BaseSessionManager::getBaseSession(B1BaseSocket* baseSocket) const
{
    B1AutoLock al(_sessionsLock);
    auto itr = _sessions.find(baseSocket);
    return itr != _sessions.end() ? itr->second._session : std::shared_ptr<B1BaseSession>();
}

std::shared_ptr<B1BaseSession> B1BaseSessionManager::getBaseSessionByHandleID(int32 handleID) const
{
    B1BaseSocket* baseSocket = NULL;
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
