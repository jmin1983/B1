//
// B1MailSender.cpp
//
// Library: B1SMTP
// Package: B1SMTP
// Module:  B1SMTP
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SMTP.h"
#include "B1MailSender.h"
#include "B1SMTPClient.h"

using namespace BnD;

B1MailSender::B1MailSender()
    : _lastResult(SEND_RESULT_FAIL)
{
}

B1MailSender::~B1MailSender()
{
}

bool B1MailSender::sendHello()
{
    if (_smtpClient->sendHello(_serverAddress) != true) {
        _lastResult = SEND_RESULT_FAIL_RECEIVE_HELLO;
        return false;
    }
    return true;
}

bool B1MailSender::sendMessage(const B1Mail& mail)
{
    return true;
}

bool B1MailSender::initialize(const B1String& serverAddress, uint16 serverPort)
{
    if (_smtpClient != NULL) {
        return false;
    }
    _smtpClient = std::make_shared<B1SMTPClient>();
    if (_smtpClient->initialize(serverAddress, serverPort) != true) {
        _smtpClient->finalize();
        _smtpClient.reset();
    }
    _serverAddress = serverAddress.copy();
    return true;
}

void B1MailSender::finalize()
{
    if (_smtpClient) {
        _smtpClient->finalize();
        _smtpClient.reset();
    }
    _serverAddress.clear();
}

bool B1MailSender::sendMail(const std::shared_ptr<B1Mail> mail)
{
    if (sendHello() != true) {
        return false;
    }
    _lastResult = SEND_RESULT_SUCCEED;
    return true;
}

bool B1MailSender::sendMail(const std::vector<std::shared_ptr<B1Mail> >& mails)
{
    return true;
}
