//
// B1SECS2ServerSessionImplS9Send.cpp
//
// Library: B1SECS2
// Package: Server
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2ServerSession.h"
#include "B1SECS2MessageS9.h"

using namespace BnD;

void B1SECS2ServerSession::sendMessageS9F1(uint16 sessionID, const std::vector<uint8>& systemBytes, const std::vector<uint8>& messageHeader)
{
    B1SECS2MessageS9F1 messageS9F1(messageHeader);
    if (messageS9F1.writeData() != true)
        return;
    sendDataMessage(sessionID, systemBytes, messageS9F1);
}

void B1SECS2ServerSession::sendMessageS9F3(uint16 sessionID, const std::vector<uint8>& systemBytes, const std::vector<uint8>& messageHeader)
{
    B1SECS2MessageS9F3 messageS9F3(messageHeader);
    if (messageS9F3.writeData() != true)
        return;
    sendDataMessage(sessionID, systemBytes, messageS9F3);
}

void B1SECS2ServerSession::sendMessageS9F5(uint16 sessionID, const std::vector<uint8>& systemBytes, const std::vector<uint8>& messageHeader)
{
    B1SECS2MessageS9F5 messageS9F5(messageHeader);
    if (messageS9F5.writeData() != true)
        return;
    sendDataMessage(sessionID, systemBytes, messageS9F5);
}

void B1SECS2ServerSession::sendMessageS9F7(uint16 sessionID, const std::vector<uint8>& systemBytes, const std::vector<uint8>& messageHeader)
{
    B1SECS2MessageS9F7 messageS9F7(messageHeader);
    if (messageS9F7.writeData() != true)
        return;
    sendDataMessage(sessionID, systemBytes, messageS9F7);
}

void B1SECS2ServerSession::sendMessageS9F9(uint16 sessionID, const std::vector<uint8>& systemBytes, const std::vector<uint8>& messageHeader)
{
    B1SECS2MessageS9F9 messageS9F9(messageHeader);
    if (messageS9F9.writeData() != true)
        return;
    sendDataMessage(sessionID, systemBytes, messageS9F9);
}

void B1SECS2ServerSession::sendMessageS9F11(uint16 sessionID, const std::vector<uint8>& systemBytes, const std::vector<uint8>& messageHeader)
{
    B1SECS2MessageS9F11 messageS9F11(messageHeader);
    if (messageS9F11.writeData() != true)
        return;
    sendDataMessage(sessionID, systemBytes, messageS9F11);
}
