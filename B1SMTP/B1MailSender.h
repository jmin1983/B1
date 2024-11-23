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
            SEND_RESULT_FAIL_TO_SEND_MESSAGE = -6,
            SEND_RESULT_FAIL_TO_SEND_CMD_FROM = -5,
            SEND_RESULT_FAIL_TO_SEND_CMD_RCPT = -4,
            SEND_RESULT_FAIL_TO_SEND_CMD_DATA = -3,
            SEND_RESULT_FAIL_RECEIVE_HELLO = -2,
            SEND_RESULT_FAIL = -1,
            SEND_RESULT_SUCCEED = 0,
        };
    protected:
        std::shared_ptr<B1SMTPClient> _smtpClient;
        B1String _serverAddress;
        SEND_RESULT _lastResult;
    protected:
        bool sendHello();
        bool sendMessage(const B1Mail& mail);
    public:
        bool initialize(const B1String& serverAddress, uint16 serverPort);  //  only support single session.
        void finalize();
        bool sendMail(const std::shared_ptr<B1Mail> mail);
        bool sendMail(const std::vector<std::shared_ptr<B1Mail> >& mails);

        SEND_RESULT lastResult() const { return _lastResult; }
    };
}   //  !BnD

#endif  // !_B1SMTP_MAIL_SENDER_H
