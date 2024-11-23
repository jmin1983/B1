//
// B1SMTPClientSession.h
//
// Library: B1SMTP
// Package: B1SMTP
// Module:  B1SMTP
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SMTP_SMTP_CLIENT_SESSION_H
#define _B1SMTP_SMTP_CLIENT_SESSION_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1ArrayBufferClientSession.h>
#include <B1SMTP/B1SMTPPacketAnalyzer.h>

namespace BnD {
    class B1Mail;
    class B1SMTPPacketMaker;
    class B1SMTPClientSession : protected B1SMTPPacketAnalyzer
                              , public B1ArrayBufferClientSession {
    public:
        B1SMTPClientSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* listener, B1SMTPPacketMaker* packetMaker);
        virtual ~B1SMTPClientSession();
    protected:
        enum CONSTS {
            CONSTS_RESPONSE_TIME_OUT = 1000 * 5,
        };
    private:
        bool _remoteServiceReady;
        bool _lastActionOk;
    protected:
        B1SMTPPacketMaker* _packetMaker;
    protected:  //  B1SMTPPacketAnalyzer
        virtual void implOnRecvSMTPResponseServiceReady(B1String&& message) override;
        virtual void implOnRecvSMTPResponseActionOK(B1String&& message) override;
    protected:  //  B1ArrayBufferClientSession
        void onReadComplete(uint8* data, size_t dataSize) final;
        virtual void implOnConnect() override;
        virtual void implOnDisconnected(int32 reason) override;
    public:
        bool sendHello(const B1String& serverName);
        bool sendMessage(const B1Mail& mail);
    };
}   //  !BnD

#endif  // !_B1SMTP_SMTP_CLIENT_SESSION_H
