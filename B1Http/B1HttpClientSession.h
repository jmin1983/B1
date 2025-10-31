//
// B1HttpClientSession.h
//
// Library: B1Http
// Package: Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1HTTP_HTTP_CLIENT_SESSION_H
#define _B1HTTP_HTTP_CLIENT_SESSION_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1BaseClientSession.h>
#include <B1Http/B1HttpClientReadWriteImpl.h>

namespace BnD {
    class B1HttpClientSession : protected B1HttpClientReadWriteImplListener
                              , public B1BaseClientSession {
    public:
        B1HttpClientSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* baseClientSessionListener);
        virtual ~B1HttpClientSession();
    protected:
        virtual void onHttpResponseSuccess(const B1String& body) {}
        virtual void onHttpResponseFail(int32 reason) {}
    protected:  //  B1HttpClientReadWriteImplListener
        void onReadComplete(const B1HttpClientMessage& httpMessage) final;
        void onReadFailed(int32 reason) final;
        void onWriteFailed(int32 reason) final;
    protected:  //  B1BaseClientSession
        virtual B1BaseReadWriteImpl* createReadWriteImpl() override;
    protected:
        B1HttpClientReadWriteImpl* readWriteImpl() const;
    public:
        void sendHttpRequest(const B1String& target);
    };
}   //  !BnD

#endif  // !_B1HTTP_HTTP_CLIENT_SESSION_H
