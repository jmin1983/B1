//
// B1WebSocketClientSession.h
//
// Library: B1Http
// Package: Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1HTTP_WEB_SOCKET_CLIENT_SESSION_H
#define _B1HTTP_WEB_SOCKET_CLIENT_SESSION_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1BaseClientSession.h>
#include <B1Http/B1WebSocketReadWriteImpl.h>

namespace BnD {
    class B1HttpClientMessage;
    class B1WebSocketClientSession : protected B1WebSocketReadWriteImplListener
                                   , public B1BaseClientSession {
    public:
        B1WebSocketClientSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* listener);
        virtual ~B1WebSocketClientSession();
    protected:  //  B1WebSocketReadWriteImplListener
        void onReadFailed(int32 reason) final;
        void onWriteFailed(int32 reason) final;
    protected:  //  B1BaseServerSession
        virtual B1BaseReadWriteImpl* createReadWriteImpl() override;
        virtual void implOnConnect() override;
    private:
        B1WebSocketReadWriteImpl* readWriteImpl() const;
    public:
        void writeBinary(std::vector<uint8>&& data) const;
        void writeText(B1String&& text) const;
        void setTextBunchHint(uint32 value);
    };
}   //  !BnD

#endif  // !_B1HTTP_WEB_SOCKET_CLIENT_SESSION_H
