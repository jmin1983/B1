//
// B1HttpResponseGenerator.h
//
// Library: B1Http
// Package: B1Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1HTTP_HTTP_RESPONSE_GENERATOR_H
#define _B1HTTP_HTTP_RESPONSE_GENERATOR_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    class B1HttpMessage;
    class B1HttpResponseAPIGenerator;
    class B1HttpResponseGenerator {
    public:
        B1HttpResponseGenerator();
        virtual ~B1HttpResponseGenerator();
    protected:
        boost::beast::http::message_generator makeResponseBadRequest(const B1HttpMessage& message, const B1String& why);
        boost::beast::http::message_generator makeResponseNotFound(const B1HttpMessage& message);
        boost::beast::http::message_generator makeResponseServerError(const B1HttpMessage& message, const B1String& what);
        boost::beast::http::message_generator makeResponseContents(const B1HttpMessage& message, const B1String& contentsRootPath);
        boost::beast::http::message_generator makeResponseAPI(const B1HttpMessage& message, const B1String& apiName);
        boost::beast::string_view getMimeType(boost::beast::string_view path);
        B1String getRequestedAPIName(const boost::beast::string_view& target) const;   //  return empty if not a API.
        B1String getRequestedContentsFullPath(const B1String& contentsRootPath, const boost::beast::string_view& target);
    public:
        boost::beast::http::message_generator makeResponse(const B1HttpMessage& message, const B1String& contentsRootPath);
    };
}   //  !BnD

#endif  // !_B1HTTP_HTTP_RESPONSE_GENERATOR_H
