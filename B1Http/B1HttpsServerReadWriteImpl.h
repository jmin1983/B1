//
// B1HttpsServerReadWriteImpl.h
//
// Library: B1Http
// Package: Https
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1HTTP_HTTPS_READ_WRITE_IMPL_H
#define _B1HTTP_HTTPS_READ_WRITE_IMPL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Http/B1HttpServerReadWriteImpl.h>

namespace BnD {
    class B1SSLContext;
    class B1HttpsServerReadWriteImpl : public B1HttpServerReadWriteImpl {
    public:
        B1HttpsServerReadWriteImpl(const B1SSLContext& sslContext, class B1HttpServerReadWriteImplListener* listener);
    protected:
        const B1SSLContext& _sslContext;
    protected:
        void implWriteResponse(boost::beast::http::message_generator&& response) final;
    protected:
        B1BaseSocketImpl* createBaseSocketImpl() final;
        bool implRead() final;  //  handshake.
    protected:
        void handshakeComplete(const boost::system::error_code& error, size_t bytesUsed);
        void readAfterHandshake();
    protected:
        class B1SecureSocketImpl* secureSocketImpl() const;
    };
}   //  !BnD

#endif  // !_B1HTTP_HTTPS_READ_WRITE_IMPL_H
