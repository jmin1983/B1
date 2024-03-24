//
// B1HttpMessage.h
//
// Library: B1Http
// Package: B1Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1HTTP_HTTP_MESSAGE_H
#define _B1HTTP_HTTP_MESSAGE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    class B1HttpMessage {
    public:
        B1HttpMessage();
        virtual ~B1HttpMessage();
    protected:
        boost::beast::flat_buffer _buffer;
        boost::beast::http::request<boost::beast::http::string_body> _request;
    public:
        boost::beast::flat_buffer& buffer() { return _buffer; }
              boost::beast::http::request<boost::beast::http::string_body>& request()       { return _request; }
        const boost::beast::http::request<boost::beast::http::string_body>& request() const { return _request; }
        void clearRequest();
    };
}   //  !BnD

#endif  // !_B1HTTP_HTTP_MESSAGE_H
