//
// SECS2ClientSessionEtc.cpp
//
// Library: B1SECS2
// Package: Client
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2ClientSession.h"

using namespace BnD;

void B1SECS2ClientSession::onRecvMessageS1F1(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes)
{
    assert(false);
    sendMessageS1F0(sessionID, systemBytes);
}
