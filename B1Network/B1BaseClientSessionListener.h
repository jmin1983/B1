//
// B1BaseClientSessionListener.h
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1NETWORK_BASE_CLIENT_SESSION_LISTENER_H
#define _B1NETWORK_BASE_CLIENT_SESSION_LISTENER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    class B1ClientSocket;
    class B1BaseClientSessionListener {
    public:
        virtual void onClientSessionConnectResult(B1ClientSocket* clientSocket, bool isConnected, int32 reason) {}  //  call BEFORE connect (success or fail) complete.
        virtual void onClientSessionClosed(B1ClientSocket* clientSocket) {}
    };
}   //  !BnD

#endif  // !_B1NETWORK_BASE_CLIENT_SESSION_LISTENER_H
