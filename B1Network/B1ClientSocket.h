//
// B1ClientSocket.h
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1NETWORK_CLIENT_SOCKET_H
#define _B1NETWORK_CLIENT_SOCKET_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1BaseSocket.h>

namespace boost {
    namespace system {
        class error_code;
    };
};

namespace BnD {
    class B1ClientSocketListener;
    class B1IOLooper;
    class B1ClientSocket : public B1BaseSocket {
    public:
        B1ClientSocket();
        virtual ~B1ClientSocket();
    protected:
        std::shared_ptr<B1IOLooper> _ioLooper;
        B1ClientSocketListener* _listener;
        B1String _address;
        uint16 _port;
    private:
        void connectComplete(const boost::system::error_code& error);
    protected:
        bool connectAsync();
    public:
        bool connect(B1String&& address, uint16 port, B1ClientSocketListener* listener);
        bool reconnect();
    };
}   //  !BnD

#endif  // !_B1NETWORK_CLIENT_SOCKET_H
