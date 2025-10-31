//
// B1HttpClientReadWriteImpl.h
//
// Library: B1Http
// Package: Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1HTTP_HTTP_CLIENT_READ_WRITE_IMPL_H
#define _B1HTTP_HTTP_CLIENT_READ_WRITE_IMPL_H

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
    class B1HttpClientMessage;
    class B1HttpClientReadWriteImplListener : public B1BaseReadWriteImplListener {
    public:
        virtual void onReadComplete(const B1HttpClientMessage& httpMessage) {}
    };

    class B1HttpClientReadWriteImpl : public B1BaseReadWriteImpl {
    public:
        B1HttpClientReadWriteImpl(B1HttpClientReadWriteImplListener* listener);
    protected:
        std::shared_ptr<B1HttpClientMessage> _httpMessage;
    private:
        class B1HttpSocketImpl* httpSocketImpl() const;
    protected:
        void writeRequestComplete(const boost::system::error_code& error, size_t transferredBytes);
    protected:
        virtual B1BaseSocketImpl* createBaseSocketImpl() override;
        virtual bool implRead() override;
        bool implOnReadComplete(size_t receivedBytes) final;    //  return false if there are no more data to read.
    protected:
        B1HttpClientReadWriteImplListener* listener() const;
    public:
        void writeRequest(const B1String& target);
    };
}   //  !BnD

#endif  // !_B1HTTP_HTTP_CLIENT_READ_WRITE_IMPL_H
