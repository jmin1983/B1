//
// B1SecureWebSocketReadWriteImpl.h
//
// Library: B1Http
// Package: https
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1HTTP_SECURE_WEB_SOCKET_READ_WRITE_IMPL_H
#define _B1HTTP_SECURE_WEB_SOCKET_READ_WRITE_IMPL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Http/B1WebSocketReadWriteImpl.h>

namespace BnD {
    class B1SSLContext;
    class B1SecureWebSocketReadWriteImpl : public B1WebSocketReadWriteImpl {
    public:
        B1SecureWebSocketReadWriteImpl(const B1SSLContext& sslContext, B1WebSocketReadWriteImplListener* listener);
        virtual ~B1SecureWebSocketReadWriteImpl();
    protected:
        const B1SSLContext& _sslContext;
    protected:
        void implWriteBinary(std::vector<uint8>&& data) final;
        void implWriteText(B1String&& data) final;
    protected:
        B1BaseSocketImpl* createBaseSocketImpl() final;
        bool implRead() final;
        bool implOnReadComplete(size_t receivedBytes) final;    //  return false if there are no more data to read.
    public:
        class B1SecureWebSocketImpl* secureWebSocketImpl() const;
    };
}   //  !BnD

#endif  // !_B1HTTP_SECURE_WEB_SOCKET_READ_WRITE_IMPL_H
