//
// B1BaseServerSession.h
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1NETWORK_BASE_SERVER_SESSION_H
#define _B1NETWORK_BASE_SERVER_SESSION_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1BaseSession.h>

namespace BnD {
    class B1BaseServerSessionListener;
    class B1ServerSocket;
    class B1BaseServerSession : public B1BaseSession {
    public:
        B1BaseServerSession(B1ServerSocket* serverSocket, B1BaseServerSessionListener* listener);
        virtual ~B1BaseServerSession();
    protected:
        B1BaseServerSessionListener* _listener;
    protected:
        virtual void implOnServerSessionDisconnected(int32 reason) {}
    protected:
        void implOnDisconnected(int32 reason) final;
    protected:
        B1ServerSocket* serverSocket() const;
    public:
        bool beginRead();
    };
}   //  !BnD

#endif  // !_B1NETWORK_BASE_SERVER_SESSION_H
