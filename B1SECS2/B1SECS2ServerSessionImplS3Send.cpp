//
// B1SECS2ServerSessionImplS2Send.cpp
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
#include "B1SECS2MessageS3F18Writable.h"

using namespace BnD;

void B1SECS2ServerSession::sendMessageS3F18(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataCAACK& caAck, const std::map<B1SECS2DataERRCODE, B1SECS2DataERRTEXT>& errorText)
{
    B1SECS2MessageS3F18Writable messageS3F18(caAck, errorText);
    if (messageS3F18.writeData() != true) {
        writeLog("sendMessageS3F0");
        sendMessageS3F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, messageS3F18);
}
