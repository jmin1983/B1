//
// B1HttpsServerSession.h
//
// Library: B1Http
// Package: Https
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1HTTP_HTTPS_SERVER_SESSION_H
#define _B1HTTP_HTTPS_SERVER_SESSION_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Http/B1HttpServerSession.h>

namespace BnD {
    class B1SSLContext;
    class B1HttpsServerSession : public B1HttpServerSession {
    public:
        B1HttpsServerSession(B1ServerSocket* serverSocket, B1BaseServerSessionListener* baseServerSessionListener,
                             B1HttpServerSessionListener* httpServerSessionListener, const B1String& contentsRootPath, const B1SSLContext& sslContext);
        virtual ~B1HttpsServerSession();
    protected:
        const B1SSLContext& _sslContext;
    protected:  //  B1BaseServerSession
        B1BaseReadWriteImpl* createReadWriteImpl() final;
    protected:
        class B1HttpsServerReadWriteImpl* readWriteImpl() const;
    };
}   //  !BnD

#endif  // !_B1HTTP_HTTPS_SERVER_SESSION_H
