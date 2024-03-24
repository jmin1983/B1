//
// B1BaseClientSession.h
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1NETWORK_BASE_CLIENT_SESSION_H
#define _B1NETWORK_BASE_CLIENT_SESSION_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1BaseSession.h>
#include <B1Network/B1ClientSocketListener.h>

namespace BnD {
    class B1BaseClientSessionListener;
    class B1ClientSocket;
    class B1BaseClientSession : protected B1ClientSocketListener
                              , public B1BaseSession {
    public:
        B1BaseClientSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* listener);
        virtual ~B1BaseClientSession();
    private:
        bool _closeConnection;
    protected:
        B1BaseClientSessionListener* _listener;
    protected:  //  B1ClientSocketListener
        void onConnectComplete() final;
        void onConnectFailed(int32 reason) final;
    protected:  //  B1BaseSession
        virtual void implOnConnectionStatusChanged(STATUS previousStatus, STATUS newStatus) override;
        virtual void implProcessDisconnected() override;
    protected:
        B1ClientSocket* clientSocket() const;
        void reconnect();
    public:
        bool connect(B1String&& address, uint16 port);
        void closeSession();
    };
}   //  !BnD

#endif  // !_B1NETWORK_BASE_CLIENT_SESSION_H
