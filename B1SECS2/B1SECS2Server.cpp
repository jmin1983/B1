//
// B1SECS2Server.cpp
//
// Library: B1SECS2
// Package: Server
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2Server.h"
#include "B1SECS2ServerSession.h"

using namespace BnD;

B1SECS2Server::B1SECS2Server(uint16 deviceID)
    : B1BaseServer()
    , _deviceID(deviceID)
    , _serverSecs2DataManager()
{
}

B1SECS2Server::~B1SECS2Server()
{
}

B1BaseServerSession* B1SECS2Server::createSession(B1ServerSocket* serverSocket)
{
    return new B1SECS2ServerSession(serverSocket, this, _deviceID, this,
                                    B1HSMSMessage::TIME_OUT_DEFAULT_T3,
                                    B1HSMSMessage::TIME_OUT_DEFAULT_T6,
                                    B1HSMSMessage::TIME_OUT_DEFAULT_T7,
                                    B1HSMSMessage::TIME_OUT_DEFAULT_T8,
                                    B1HSMSMessage::TIME_OUT_DEFAULT_LINK_TEST);
}

bool B1SECS2Server::implInitialize(int32 port)
{
    _serverSecs2DataManager = createServerSECS2DataManager();
    if (_serverSecs2DataManager == NULL) {
        return false;
    }
    if (_serverSecs2DataManager->initialize() != true) {
        return false;
    }
    if (port < 1 || port > UINT16_MAX) {
        return false;
    }
    if (startup() != true) {
        return false;
    }
    if (accept(static_cast<uint16>(port)) != true) {
        return false;
    }
    return true;
}

void B1SECS2Server::implFinalize()
{
    close();
    shutdown();
    if (_serverSecs2DataManager) {
        _serverSecs2DataManager->finalize();
        _serverSecs2DataManager.reset();
    }
}

bool B1SECS2Server::initialize(int32 port)
{
    return implInitialize(port);
}

void B1SECS2Server::finalize()
{
    implFinalize();
}
