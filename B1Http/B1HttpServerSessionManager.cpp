//
// B1HttpServerSessionManager.cpp
//
// Library: B1Http
// Package: B1Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Http.h"
#include "B1HttpServerSessionManager.h"

using namespace BnD;

B1HttpServerSessionManager::B1HttpServerSessionManager()
{
}

B1HttpServerSessionManager::~B1HttpServerSessionManager()
{
}

void B1HttpServerSessionManager::onShuttingDown()
{
    B1AutoLock al(_webSocketSessionsLock);
    _webSocketSessions.clear();
}

void B1HttpServerSessionManager::onSessionModified(int32 handleID, std::shared_ptr<B1BaseSession> session)
{
    B1AutoLock al(_webSocketSessionsLock);
    _webSocketSessions[handleID] = session;
}

void B1HttpServerSessionManager::onSessionRemoved(int32 handleID)
{
    B1AutoLock al(_webSocketSessionsLock);
    _webSocketSessions.erase(handleID);
}

auto B1HttpServerSessionManager::webSocketSessions() const -> std::map<int32, std::shared_ptr<B1BaseSession> >
{
    B1AutoLock al(_webSocketSessionsLock);
    return _webSocketSessions;
}

auto B1HttpServerSessionManager::webSocketSessionByHandleID(int32 handleID) const->std::shared_ptr<B1BaseSession>
{
    B1AutoLock al(_webSocketSessionsLock);
    auto itr = _webSocketSessions.find(handleID);
    return itr != _webSocketSessions.end() ? itr->second : std::shared_ptr<B1BaseSession>();
}
