//
// B1HttpsServer.h
//
// Library: B1Http
// Package: Https
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1HTTP_HTTPS_SERVER_H
#define _B1HTTP_HTTPS_SERVER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Http/B1HttpServer.h>

namespace BnD {
    class B1HttpsServer : public B1HttpServer {
    public:
        B1HttpsServer(B1String&& contentsRootPath);
    protected:
        std::shared_ptr<class B1SSLContext> _sslContext;
    protected:
        virtual B1String sslCert() = 0;
        virtual B1String sslKey() = 0;
        virtual B1String sslDh() = 0;
        virtual B1WebSocketServerSession* createWebSocketServerSession(B1ServerSocket* serverSocket) override;
    protected:
        virtual B1BaseServerSession* createSession(B1ServerSocket* serverSocket) override;
    protected:
        virtual bool implInitialize(int32 port) override;
    };
}   //  !BnD

#endif  // !_B1HTTP_HTTPS_SERVER_H
