//
// B1HttpReadWriteImpl.h
//
// Library: B1Http
// Package: B1Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1HTTP_HTTP_READ_WRITE_IMPL_H
#define _B1HTTP_HTTP_READ_WRITE_IMPL_H

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
    class B1HttpMessage;
    class B1HttpReadWriteImplListener : public B1BaseReadWriteImplListener {
    public:
        virtual void onReadComplete(const B1HttpMessage& httpMessage) {}
        virtual void onWriteComplete(bool keepAlive, size_t dataSize) {}
        virtual void onRequestWebSocketUpgrade() {}
    };

    class B1HttpReadWriteImpl : public B1BaseReadWriteImpl {
    public:
        B1HttpReadWriteImpl(B1BaseSocket* baseSocket, B1HttpReadWriteImplListener* listener);
    protected:
        std::shared_ptr<B1HttpMessage> _httpMessage;
    private:
        void writeResponseComplete(bool keepAlive, const boost::system::error_code& error, size_t transferredBytes);
    protected:
        bool implRead() final;
        bool implOnReadComplete(size_t receivedBytes) final;    //  return false if there are no more data to read.
    protected:
        B1HttpReadWriteImplListener* listener() const;
    public:
        void writeResponse(boost::beast::http::message_generator&& response);
    };
}   //  !BnD

#endif  // !_B1HTTP_HTTP_READ_WRITE_IMPL_H
