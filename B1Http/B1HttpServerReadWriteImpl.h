//
// B1HttpServerReadWriteImpl.h
//
// Library: B1Http
// Package: Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1HTTP_HTTP_SERVER_READ_WRITE_IMPL_H
#define _B1HTTP_HTTP_SERVER_READ_WRITE_IMPL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1BaseReadWriteImpl.h>

namespace boost {
    namespace system {
        class error_code;
    };
    namespace beast {
        namespace http {
            class message_generator;
        };
    };
};

namespace BnD {
    class B1HttpServerMessage;
    class B1HttpServerReadWriteImplListener : public B1BaseReadWriteImplListener {
    public:
        virtual void onReadComplete(const B1HttpServerMessage& httpMessage) {}
        virtual void onWriteComplete(bool keepAlive, size_t dataSize) {}
    };

    class B1HttpServerReadWriteImpl : public B1BaseReadWriteImpl {
    public:
        B1HttpServerReadWriteImpl(B1HttpServerReadWriteImplListener* listener);
    protected:
        std::shared_ptr<B1HttpServerMessage> _httpMessage;
    private:
        class B1ASIOSocketImpl* asioSocketImpl() const;
    protected:
        void writeResponseComplete(bool keepAlive, const boost::system::error_code& error, size_t transferredBytes);
    protected:
        virtual void implWriteResponse(boost::beast::http::message_generator&& response);
    protected:
        virtual bool implRead() override;
        bool implOnReadComplete(size_t receivedBytes) final;    //  return false if there are no more data to read.
    protected:
        B1HttpServerReadWriteImplListener* listener() const;
    public:
        void writeResponse(boost::beast::http::message_generator&& response);
    };
}   //  !BnD

#endif  // !_B1HTTP_HTTP_SERVER_READ_WRITE_IMPL_H
