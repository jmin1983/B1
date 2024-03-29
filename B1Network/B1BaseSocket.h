//
// B1BaseSocket.h
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1NETWORK_BASE_SOCKET_H
#define _B1NETWORK_BASE_SOCKET_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    class B1BaseSocketImpl;
    class B1IOContext;
    class B1BaseSocket {
    public:
        B1BaseSocket();
        virtual ~B1BaseSocket();
    private:
        B1BaseSocketImpl* _impl;
        std::shared_ptr<boost::asio::ip::tcp::socket> _asioSocket;
    protected:
        void resetSocket(const B1IOContext* context);
    public:
        void close();
        B1String peerAddress() const;
        uint16 peerPort() const;
        uint16 localPort() const;
        bool isOpen() const;
        void setImpl(B1BaseSocketImpl* impl);
        void resetImpl();
    public:
        boost::asio::ip::tcp::socket* asioSocket() const { return _asioSocket.get(); }  //  for convenience use only.
    };
}   //  !BnD

#endif  // !_B1NETWORK_BASE_SOCKET_H
