//
// B1MailSender.h
//
// Library: B1SMTP
// Package: B1SMTP
// Module:  B1SMTP
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SMTP_MAIL_SENDER_H
#define _B1SMTP_MAIL_SENDER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    class B1Mail;
    class B1SMTPClient;
    class B1MailSender {
    public:
        B1MailSender();
        virtual ~B1MailSender();
    public:
        enum SEND_RESULT {
            SEND_RESULT_FAIL_AUTH = -104,
            SEND_RESULT_FAIL_SEND_USER_PASSWORD = -103,
            SEND_RESULT_FAIL_SEND_USER_ID = -102,
            SEND_RESULT_FAIL_SEND_AUTH = -101,

            SEND_RESULT_FAIL_QUIT = -9,
            SEND_RESULT_FAIL_SEND_CONTENTS = -8,
            SEND_RESULT_FAIL_SEND_START_MAIL_INPUT = -7,
            SEND_RESULT_FAIL_SEND_RCPT_BCC = -6,
            SEND_RESULT_FAIL_SEND_RCPT_CC = -5,
            SEND_RESULT_FAIL_SEND_RCPT_TO = -4,
            SEND_RESULT_FAIL_SEND_FAIL_FROM = -3,
            SEND_RESULT_FAIL_SEND_HELLO = -2,
            SEND_RESULT_FAIL = -1,
            SEND_RESULT_SUCCEED = 0,
            SEND_RESULT_QUIT_BUT_COMPLETE = 1,
        };
    protected:
        SEND_RESULT _lastResult;
        std::shared_ptr<B1SMTPClient> _smtpClient;
        B1String _serverAddress;
        B1String _userID;
        B1String _userPassword;
    protected:
        bool useAuth() const { return _userID.isEmpty() != true; }
    public:
        bool initialize(const B1String& serverAddress, uint16 serverPort);  //  only support single session.
        bool initialize(const B1String& serverAddress, uint16 serverPort, B1String&& userID, B1String&& userPassword);  //  only support single session.
        void finalize();
        bool sendMail(const B1Mail& mail);
        SEND_RESULT lastResult() const { return _lastResult; }
    };
}   //  !BnD

#endif  // !_B1SMTP_MAIL_SENDER_H
