//
// B1HttpServerSession.h
//
// Library: B1Http
// Package: B1Http
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
#include <B1Http/B1HttpReadWriteImpl.h>

namespace BnD {
    class B1HttpMessage;
    class B1HttpResponseGenerator;
    class B1HttpServerSessionListener;
    class B1HttpServerSession : protected B1HttpReadWriteImplListener
                              , public B1BaseServerSession {
    public:
        B1HttpServerSession(B1ServerSocket* serverSocket, B1BaseServerSessionListener* baseServerSessionListener,
                            B1HttpServerSessionListener* httpServerSessionListener, const B1String& contentsRootPath);
        virtual ~B1HttpServerSession();
    private:
        B1HttpServerSessionListener* _httpServerSessionListener;
        const B1String& _contentsRootPath;
    protected:
        std::shared_ptr<B1HttpResponseGenerator> _responseGenerator;
    protected:  //  B1HttpReadWriteImplListener
        void onReadComplete(const B1HttpMessage& httpMessage) final;
        void onWriteComplete(bool keepAlive, size_t dataSize) final;
        void onReadFailed(int32 reason) final;
        void onWriteFailed(int32 reason) final;
    protected:  //  B1BaseServerSession
        B1BaseReadWriteImpl* createReadWriteImpl(B1BaseSocket* baseSocket) final;
    protected:
        B1HttpReadWriteImpl* readWriteImpl() const;
    };
}   //  !BnD

#endif  // !_B1HTTP_HTTP_SERVER_SESSION_H
