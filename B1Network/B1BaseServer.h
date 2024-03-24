//
// B1BaseServer.h
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1NETWORK_BASE_SERVER_H
#define _B1NETWORK_BASE_SERVER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1AcceptorListener.h>
#include <B1Network/B1BaseComponent.h>
#include <B1Network/B1BaseServerSessionListener.h>

namespace BnD {
    class B1Acceptor;
    class B1BaseServerSession;
    class B1IOLooper;
    class B1ServerSocket;
    class B1BaseServer : protected B1AcceptorListener
                       , protected B1BaseServerSessionListener
                       , public B1BaseComponent {
    public:
        B1BaseServer();
        virtual ~B1BaseServer();
    protected:
        std::shared_ptr<B1Acceptor> _acceptor;
        std::shared_ptr<B1IOLooper> _ioLooper;
    protected:
        virtual B1BaseServerSession* createSession(B1ServerSocket* serverSocket) = 0;
        virtual bool isAcceptableSession(B1ServerSocket* serverSocket) { return true; }
    protected:  //  B1AcceptorListener
        void onAcceptComplete(std::shared_ptr<B1ServerSocket> serverSocket) final;
        void onAcceptFailed(std::shared_ptr<B1ServerSocket> serverSocket, int32 reason) final;
    protected:  //  B1BaseServerSessionListener
        virtual void onServerSessionDisconnected(B1ServerSocket* serverSocket, int32 reason) override;
    protected:  //  B1BaseComponent
        bool implStartup() final;
        void implShutdown() final;
    public:
        bool accept(uint16 port);
        void close();
        void disconnect(B1ServerSocket* serverSocket);
        void disconnectAllSessions();
        B1String peerAddress(B1ServerSocket* serverSocket) const;
        uint16 localPort(B1ServerSocket* serverSocket) const;
    };
}   //  !BnD

#endif  // !_B1NETWORK_BASE_SERVER_H
