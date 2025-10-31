//
// B1HttpServerSession.h
//
// Library: B1Http
// Package: Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1HTTP_HTTP_SERVER_SESSION_H
#define _B1HTTP_HTTP_SERVER_SESSION_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1BaseServerSession.h>
#include <B1Http/B1HttpServerReadWriteImpl.h>

namespace BnD {
    class B1HttpResponseGenerator;
    class B1HttpServerSessionListener;
    class B1HttpServerSession : protected B1HttpServerReadWriteImplListener
                              , public B1BaseServerSession {
    public:
        B1HttpServerSession(B1ServerSocket* serverSocket, B1BaseServerSessionListener* baseServerSessionListener,
                            B1HttpServerSessionListener* httpServerSessionListener, const B1String& contentsRootPath);
        virtual ~B1HttpServerSession();
    private:
        B1HttpServerSessionListener* _httpServerSessionListener;
    protected:
        const B1String& _contentsRootPath;
        std::shared_ptr<B1HttpResponseGenerator> _responseGenerator;
    protected:  //  B1HttpServerReadWriteImplListener
        virtual void onReadComplete(const B1HttpServerMessage& httpMessage) override;
        void onWriteComplete(bool keepAlive, size_t dataSize) final;
        void onReadFailed(int32 reason) final;
        void onWriteFailed(int32 reason) final;
    protected:  //  B1BaseServerSession
        virtual B1BaseReadWriteImpl* createReadWriteImpl() override;
    protected:
        B1HttpServerReadWriteImpl* readWriteImpl() const;
    };
}   //  !BnD

#endif  // !_B1HTTP_HTTP_SERVER_SESSION_H
