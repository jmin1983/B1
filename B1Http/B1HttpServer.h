//
// B1HttpServer.h
//
// Library: B1Http
// Package: B1Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1HTTP_HTTP_SERVER_H
#define _B1HTTP_HTTP_SERVER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1BaseServer.h>
#include <B1Http/B1HttpServerSessionListener.h>

#include <map>

namespace BnD {
    class B1BaseSession;
    class B1HttpMessage;
    class B1WebSocketServerSession;
    class B1HttpServer : protected B1HttpServerSessionListener
                       , public B1BaseServer {
    public:
        B1HttpServer(B1String&& contentsRootPath);
    protected:
        const B1String _contentsRootPath;
    protected:
        virtual B1WebSocketServerSession* createWebSocketServerSession(B1ServerSocket* serverSocket);
    protected:
        void onHttpServerSessionUpgradeRequested(B1ServerSocket* serverSocket, const B1HttpMessage& httpMessage) final;
    protected:
        virtual B1BaseSessionManager* createSessionManager() override;
        virtual B1BaseServerSession* createSession(B1ServerSocket* serverSocket) override;
    protected:
        virtual bool implInitialize(int32 port);
        virtual void implFinalize();
    protected:
        std::map<int32, std::shared_ptr<B1BaseSession> > allWebSocketSessions() const;
    public:
        bool initialize(int32 port);
        void finalize();
    };
}   //  !BnD

#endif  // !_B1HTTP_HTTP_SERVER_H
