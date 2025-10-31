//
// B1HttpSocketImpl.h
//
// Library: B1Http
// Package: Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1HTTP_HTTP_SOCKET_IMPL_H
#define _B1HTTP_HTTP_SOCKET_IMPL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1BaseSocketImpl.h>

namespace BnD {
    class B1HttpSocketImpl : public B1BaseSocketImpl {
    public:
        B1HttpSocketImpl();
        virtual ~B1HttpSocketImpl();
    protected:
        std::shared_ptr<boost::beast::tcp_stream> _stream;
    protected:
        void implUpdateSocket(std::shared_ptr<boost::asio::ip::tcp::socket>* pAsioSocket) final;
        auto implRollbackSocket() -> std::shared_ptr<boost::asio::ip::tcp::socket> final;
        void implClose() final;
        B1String implPeerAddress() const final;
        uint16 implPeerPort() const final;
        uint16 implLocalPort() const final;
        bool implIsOpen() const final;
        boost::asio::ip::tcp::socket* implGetASIOSocket() const final;
    public:
              boost::beast::tcp_stream* stream()       { return _stream.get(); }
        const boost::beast::tcp_stream* stream() const { return _stream.get(); }
    };
}   //  !BnD

#endif  // !_B1HTTP_HTTP_SOCKET_IMPL_H
