//
// B1SMTPPacketMaker.cpp
//
// Library: B1SMTP
// Package: B1SMTP
// Module:  B1SMTP
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SMTP.h"
#include "B1SMTPPacketMaker.h"

using namespace BnD;

B1SMTPPacketMaker::B1SMTPPacketMaker()
{
}

B1SMTPPacketMaker::~B1SMTPPacketMaker()
{
}

std::vector<uint8> B1SMTPPacketMaker::makeRequestMessage(const B1String& message)
{
    std::vector<uint8> data;
    data.reserve(message.length() + 2);
    data.insert(data.end(), message.cString(), message.cString() + message.length());
    data.push_back('\r');
    data.push_back('\n');
    return data;
}

std::vector<uint8> B1SMTPPacketMaker::makeDataHello(const B1String& serverName, bool useAuth)
{
    B1String message;
    if (useAuth) {
        message.format("EHLO %s", serverName.cString());
    }
    else {
        message.format("HELO %s", serverName.cString());
    }
    return makeRequestMessage(message);
}
