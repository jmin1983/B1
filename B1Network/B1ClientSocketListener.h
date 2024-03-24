//
// B1ClientSocketListener.h
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1NETWORK_CLIENT_SOCKET_LISTENER_H
#define _B1NETWORK_CLIENT_SOCKET_LISTENER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    class B1ClientSocketListener {
    public:
        virtual void onConnectComplete() {}
        virtual void onConnectFailed(int32 reason) {}
    };
}   //  !BnD

#endif  // !_B1NETWORK_CLIENT_SOCKET_LISTENER_H
