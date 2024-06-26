//
// B1WebSocketImpl.h
//
// Library: B1Http
// Package: Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1HTTP_WEB_SOCKET_IMPL_H
#define _B1HTTP_WEB_SOCKET_IMPL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1BaseSocketImpl.h>

namespace BnD {
    class B1WebSocketImpl : public B1BaseSocketImpl {
    public:
        B1WebSocketImpl();
        virtual ~B1WebSocketImpl();
    protected:
        std::shared_ptr<boost::beast::websocket::stream<boost::beast::tcp_stream> > _webSocketStream;
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
              boost::beast::websocket::stream<boost::beast::tcp_stream>* webSocketStream()       { return _webSocketStream.get(); }
        const boost::beast::websocket::stream<boost::beast::tcp_stream>* webSocketStream() const { return _webSocketStream.get(); }
    };
}   //  !BnD

#endif  // !_B1HTTP_WEB_SOCKET_IMPL_H
