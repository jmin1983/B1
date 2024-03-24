//
// B1WebSocketMessage.h
//
// Library: B1Http
// Package: B1Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1HTTP_WEB_SOCKET_MESSAGE_H
#define _B1HTTP_WEB_SOCKET_MESSAGE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    class B1WebSocketMessage {
    public:
        B1WebSocketMessage();
        virtual ~B1WebSocketMessage();
    protected:
        boost::beast::flat_buffer _buffer;
    public:
              boost::beast::flat_buffer& buffer()       { return _buffer; }
        const boost::beast::flat_buffer& buffer() const { return _buffer; }
        void clearRequest();
    };
}   //  !BnD

#endif  // !_B1HTTP_WEB_SOCKET_MESSAGE_H
