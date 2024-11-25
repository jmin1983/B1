//
// B1SMTPClient.h
//
// Library: B1SMTP
// Package: B1SMTP
// Module:  B1SMTP
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SMTP_SMTP_CLIENT_H
#define _B1SMTP_SMTP_CLIENT_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1BaseClient.h>

namespace BnD {
    class B1Mail;
    class B1SMTPPacketMaker;
    class B1SMTPClient : protected B1BaseClient {
    public:
        B1SMTPClient();
        virtual ~B1SMTPClient();
    protected:
        std::shared_ptr<B1SMTPPacketMaker> _packetMaker;
    protected:
        B1BaseClientSession* createSession(B1ClientSocket* clientSocket, void* param) final;
    public:
        bool initialize(const B1String& address, uint16 port);  //  only support single session.
        void finalize();
        bool sendHello(const B1String& serverName, bool useAuth);
        bool sendAuthLogin();
        bool sendUserID(const B1String& userID);
        bool sendUserPassword(const B1String& userPassword);
        bool sendMailFrom(const B1Mail& mail);
        bool sendRcptTO(const B1Mail& mail);
        bool sendRcptCC(const B1Mail& mail);
        bool sendRcptBCC(const B1Mail& mail);
        bool sendStartMailInput();
        bool sendContents(const B1Mail& mail);
        bool sendQuit();
        bool isAuthed() const;
        bool isRemoteServiceClosed() const;
    };
}   //  !BnD

#endif  // !_B1SMTP_SMTP_CLIENT_H
