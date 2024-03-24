//
// B1HttpServerSessionListener.h
//
// Library: B1Http
// Package: B1Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1HTTP_HTTP_SERVER_SESSION_LISTENER_H
#define _B1HTTP_HTTP_SERVER_SESSION_LISTENER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    class B1HttpMessage;
    class B1ServerSocket;
    class B1HttpServerSessionListener {
    public:
        virtual void onHttpServerSessionUpgradeRequested(B1ServerSocket* serverSocket, const B1HttpMessage& httpMessage) {}
    };
}   //  !BnD

#endif  // !_B1HTTP_HTTP_SERVER_SESSION_LISTENER_H
