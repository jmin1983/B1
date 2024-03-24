//
// B1AcceptorListener.h
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1NETWORK_ACCEPTOR_LISTENER_H
#define _B1NETWORK_ACCEPTOR_LISTENER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    class B1ServerSocket;
    class B1AcceptorListener {
    public:
        virtual void onAcceptComplete(std::shared_ptr<B1ServerSocket> serverSocket) {}
        virtual void onAcceptFailed(std::shared_ptr<B1ServerSocket> serverSocket, int32 reason) {}
    };
}   //  !BnD

#endif  // !_B1NETWORK_ACCEPTOR_LISTENER_H
