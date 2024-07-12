//
// B1WebSocketServerSession.h
//
// Library: B1Http
// Package: Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1HTTP_WEB_SOCKET_SERVER_SESSION_H
#define _B1HTTP_WEB_SOCKET_SERVER_SESSION_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1BaseServerSession.h>
#include <B1Http/B1WebSocketReadWriteImpl.h>

namespace boost {
    namespace system {
        class error_code;
    };
};

namespace BnD {
    class B1HttpMessage;
    class B1WebSocketServerSession : protected B1WebSocketReadWriteImplListener
                                   , public B1BaseServerSession {
    public:
        B1WebSocketServerSession(B1ServerSocket* serverSocket, B1BaseServerSessionListener* listener);
        virtual ~B1WebSocketServerSession();
    protected:
        virtual void onWebSocketAccepted() {}
        virtual void implAcceptWebSocket(const B1HttpMessage& initialMessage);
    protected:  //  B1WebSocketReadWriteImplListener
        void onReadFailed(int32 reason) final;
        void onWriteFailed(int32 reason) final;
    protected:  //  B1BaseServerSession
        virtual B1BaseReadWriteImpl* createReadWriteImpl() override;
    private:
        B1WebSocketReadWriteImpl* readWriteImpl() const;
    protected:
        void acceptComplete(const boost::system::error_code& error);
    public:
        void acceptWebSocket(const B1HttpMessage& initialMessage);
        void writeBinary(std::vector<uint8>&& data) const;
        void writeText(B1String&& text) const;
    };
}   //  !BnD

#endif  // !_B1HTTP_WEB_SOCKET_SERVER_SESSION_H
