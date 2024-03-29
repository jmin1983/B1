//
// B1SecureWebSocketImpl.h
//
// Library: B1Http
// Package: Https
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1HTTP_SECURE_WEB_SOCKET_IMPL_H
#define _B1HTTP_SECURE_WEB_SOCKET_IMPL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1BaseSocketImpl.h>

#include <boost/beast/ssl.hpp>

namespace BnD {
    class B1SSLContext;
    class B1SecureWebSocketImpl : public B1BaseSocketImpl {
    public:
        B1SecureWebSocketImpl(const B1SSLContext& sslContext);
        virtual ~B1SecureWebSocketImpl();
    protected:
        const B1SSLContext& _sslContext;
        std::shared_ptr<boost::beast::websocket::stream<boost::beast::ssl_stream<boost::beast::tcp_stream> > > _secureWebSocketStream;
    protected:
        void implUpdateSocket(std::shared_ptr<boost::asio::ip::tcp::socket>* pAsioSocket) final;
        auto implRollbackSocket() -> std::shared_ptr<boost::asio::ip::tcp::socket> final;
        void implClose() final;
        B1String implPeerAddress() const final;
        uint16 implPeerPort() const final;
        uint16 implLocalPort() const final;
        bool implIsOpen() const final;
        bool implIsClosed() const final;
    public:
              boost::beast::websocket::stream<boost::beast::ssl_stream<boost::beast::tcp_stream> >* secureWebSocketStream()       { return _secureWebSocketStream.get(); }
        const boost::beast::websocket::stream<boost::beast::ssl_stream<boost::beast::tcp_stream> >* secureWebSocketStream() const { return _secureWebSocketStream.get(); }
    };
}   //  !BnD

#endif  // !_B1HTTP_SECURE_WEB_SOCKET_IMPL_H
