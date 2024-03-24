//
// B1ServerSocket.h
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1NETWORK_SERVER_SOCKET_H
#define _B1NETWORK_SERVER_SOCKET_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1BaseSocket.h>

namespace BnD {
    class B1ServerSocket : public B1BaseSocket {
    public:
        B1ServerSocket(const B1IOContext* context);
        virtual ~B1ServerSocket();
    };
}   //  !BnD

#endif  // !_B1NETWORK_SERVER_SOCKET_H
