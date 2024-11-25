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

#include <B1Base/B1Thread.h>

using namespace BnD;

B1MailSender::B1MailSender()
    : _lastResult(SEND_RESULT_FAIL)
{
}

B1MailSender::~B1MailSender()
{
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
    _lastResult = SEND_RESULT_FAIL;
    return true;
}

bool B1MailSender::initialize(const B1String& serverAddress, uint16 serverPort, B1String&& userID, B1String&& userPassword)
{
    if (initialize(serverAddress, serverPort) != true) {
        return false;
    }
    _userID = std::move(userID);
    _userPassword = std::move(userPassword);
    return true;
}

void B1MailSender::finalize()
{
    if (_smtpClient) {
        _smtpClient->finalize();
        _smtpClient.reset();
    }
    _userPassword.clear();
    _userID.clear();
    _serverAddress.clear();
    _lastResult = SEND_RESULT_FAIL;
}

bool B1MailSender::sendMail(const B1Mail& mail)
{
    if (_smtpClient->sendHello(_serverAddress, useAuth()) != true) {
        _lastResult = SEND_RESULT_FAIL_SEND_HELLO;
        return false;
    }
    if (useAuth()) {
        if (_smtpClient->sendAuthLogin() != true) {
            _lastResult = SEND_RESULT_FAIL_SEND_AUTH;
            return false;
        }
        if (_smtpClient->sendUserID(_userID) != true) {
            _lastResult = SEND_RESULT_FAIL_SEND_USER_ID;
            return false;
        }
        if (_smtpClient->sendUserPassword(_userPassword) != true) {
            _lastResult = SEND_RESULT_FAIL_SEND_USER_PASSWORD;
            return false;
        }
        if (_smtpClient->isAuthed() != true) {
            _lastResult = SEND_RESULT_FAIL_AUTH;
            return false;
        }
    }
    if (_smtpClient->sendMailFrom(mail) != true) {
        _lastResult = SEND_RESULT_FAIL_SEND_FAIL_FROM;
        return false;
    }
    if (_smtpClient->sendRcptTO(mail) != true) {
        _lastResult = SEND_RESULT_FAIL_SEND_RCPT_TO;
        return false;
    }
    if (_smtpClient->sendRcptCC(mail) != true) {
        _lastResult = SEND_RESULT_FAIL_SEND_RCPT_CC;
        return false;
    }
    if (_smtpClient->sendRcptBCC(mail) != true) {
        _lastResult = SEND_RESULT_FAIL_SEND_RCPT_BCC;
        return false;
    }
    if (_smtpClient->sendStartMailInput() != true) {
        _lastResult = SEND_RESULT_FAIL_SEND_START_MAIL_INPUT;
        return false;
    }
    if (_smtpClient->sendContents(mail) != true) {
        _lastResult = SEND_RESULT_FAIL_SEND_CONTENTS;
        return false;
    }
    if (_smtpClient->sendQuit() != true) {
        _lastResult = SEND_RESULT_FAIL_QUIT;
        return false;
    }
    const size_t waitCount = 10;
    bool quitSucceed = false;
    for (size_t i = 0; i < waitCount; ++i) {
        if (_smtpClient->isRemoteServiceClosed()) {
            quitSucceed = true;
            break;
        }
        B1Thread::sleep(10);
    }
    _lastResult = quitSucceed ? SEND_RESULT_SUCCEED : SEND_RESULT_QUIT_BUT_COMPLETE;
    return true;
}
