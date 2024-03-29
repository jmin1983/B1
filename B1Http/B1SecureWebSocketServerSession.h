//
// B1SecureWebSocketServerSession.h
//
// Library: B1Http
// Package: Https
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1HTTP_SECURE_WEB_SOCKET_SERVER_SESSION_H
#define _B1HTTP_SECURE_WEB_SOCKET_SERVER_SESSION_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Http/B1WebSocketServerSession.h>

namespace BnD {
    class B1HttpMessage;
    class B1SecureWebSocketServerSession : public B1WebSocketServerSession {
    public:
        B1SecureWebSocketServerSession(B1ServerSocket* serverSocket, B1BaseServerSessionListener* listener);
        virtual ~B1SecureWebSocketServerSession();
    protected:
        void implAcceptWebSocket(const B1HttpMessage& initialMessage) final;
    protected:  //  B1BaseServerSession
        B1BaseReadWriteImpl* createReadWriteImpl() final;
    protected:
        class B1SecureWebSocketReadWriteImpl* readWriteImpl() const;
    };
}   //  !BnD

#endif  // !_B1HTTP_SECURE_WEB_SOCKET_SERVER_SESSION_H
