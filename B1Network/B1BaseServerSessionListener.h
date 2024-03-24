//
// B1BaseServerSessionListener.h
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1NETWORK_BASE_SERVER_SESSION_LISTENER_H
#define _B1NETWORK_BASE_SERVER_SESSION_LISTENER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    class B1ServerSocket;
    class B1BaseServerSessionListener {
    public:
        virtual void onServerSessionDisconnected(B1ServerSocket* serverSocket, int32 reason) {}
    };
}   //  !BnD

#endif  // !_B1NETWORK_BASE_SERVER_SESSION_LISTENER_H
