//
// B1HttpClient.h
//
// Library: B1Http
// Package: Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1HTTP_HTTP_CLIENT_H
#define _B1HTTP_HTTP_CLIENT_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1BaseClient.h>

namespace BnD {
    class B1BaseHttpClient : protected B1BaseClient {
    protected:
        B1BaseHttpClient() {}
    public:
        virtual ~B1BaseHttpClient() {}
    protected:
        std::shared_ptr<class B1BaseClientSession> getSession(int32 tagID);
    public:
        bool initialize();
        void finalize();
        bool connect(B1String&& address, uint16 port, int32 tagID);
        void closeSession(int32 tagID);
    };

    class B1HttpClient : public B1BaseHttpClient {
    protected:
        virtual B1BaseClientSession* createSession(B1ClientSocket* clientSocket, void* param) override;
    protected:
        std::shared_ptr<class B1HttpClientSession> getSession(int32 tagID);
    public:
        void sendHttpRequest(int32 tagID, const B1String& target);
    };
}   //  !BnD

#endif  // !_B1HTTP_HTTP_CLIENT_H
