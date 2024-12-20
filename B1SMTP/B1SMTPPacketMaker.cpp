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

#include <B1Crypto/B1Encryptor.h>

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

B1String B1SMTPPacketMaker::makeParticipantsString(const std::list<B1Mail::B1Participant>& participants) const
{
    if (participants.empty()) {
        return "";
    }
    B1String result;
    bool first = true;
    for (const auto& participant : participants) {
        if (first) {
            first = false;
        }
        else {
            result.append(", ");
        }
        result.appendf("%s <%s>", participant.name().cString(), participant.emailAddress().cString());
    }
    return result;
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

std::vector<uint8> B1SMTPPacketMaker::makeDataAuthLogin()
{
    return makeRequestMessage("AUTH LOGIN");
}

std::vector<uint8> B1SMTPPacketMaker::makeDataBase64(const B1String& value)
{
    B1String encodedResult;
    B1Encryptor::encodeBase64(std::vector<uint8>(value.cString(), value.cString() + value.length()), &encodedResult);
    return makeRequestMessage(encodedResult);
}

std::vector<uint8> B1SMTPPacketMaker::makeDataMailFrom(const B1String& mailAddress)
{
    return makeRequestMessage("MAIL FROM:" + mailAddress);
}

std::vector<uint8> B1SMTPPacketMaker::makeDataRcptTO(const B1String& mailAddress)
{
    return makeRequestMessage("RCPT TO:" + mailAddress);
}

std::vector<uint8> B1SMTPPacketMaker::makeDataRcptCC(const B1String& mailAddress)
{
    return makeRequestMessage("RCPT CC:" + mailAddress);
}

std::vector<uint8> B1SMTPPacketMaker::makeDataRcptBCC(const B1String& mailAddress)
{
    return makeRequestMessage("RCPT BCC:" + mailAddress);
}

std::vector<uint8> B1SMTPPacketMaker::makeDataStartMailInput()
{
    return makeRequestMessage("DATA");
}

std::vector<uint8> B1SMTPPacketMaker::makeDataContents(const B1Mail& mail)
{
    B1String message;
    message.appendf("From: %s <%s>\r\n", mail.sender().name().cString(), mail.sender().emailAddress().cString());
    if (mail.receivers().empty() != true) {
        message.append("To: ");
        message.append(makeParticipantsString(mail.receivers()));
        message.append("\r\n");
    }
    if (mail.ccs().empty() != true) {
        message.append("Cc: ");
        message.append(makeParticipantsString(mail.ccs()));
        message.append("\r\n");
    }
    message.appendf("Subject: %s\r\n\r\n", mail.subject().cString());
    message.appendf("%s\r\n", mail.contents().cString());
    message.append(".");
    return makeRequestMessage(message);
}

std::vector<uint8> B1SMTPPacketMaker::makeDataQuit()
{
    return makeRequestMessage("QUIT");
}
