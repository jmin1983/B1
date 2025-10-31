//
// B1WebSocketClient.h
//
// Library: B1Http
// Package: Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1HTTP_WEB_SOCKET_CLIENT_H
#define _B1HTTP_WEB_SOCKET_CLIENT_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Http/B1HttpClient.h>

namespace BnD {
    class B1WebSocketClient : public B1BaseHttpClient {
    protected:
        virtual B1BaseClientSession* createSession(B1ClientSocket* clientSocket, void* param) override;
    protected:
        std::shared_ptr<class B1WebSocketClientSession> getSession(int32 tagID);
    public:
        bool sendBinary(int32 tagID, std::vector<uint8>&& data);
        bool sendText(int32 tagID, B1String&& text);
    };
}   //  !BnD

#endif  // !_B1HTTP_WEB_SOCKET_CLIENT_H
