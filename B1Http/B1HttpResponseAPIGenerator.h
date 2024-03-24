//
// B1HttpResponseAPIGenerator.h
//
// Library: B1Http
// Package: B1Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1HTTP_HTTP_RESPONSE_API_GENERATOR_H
#define _B1HTTP_HTTP_RESPONSE_API_GENERATOR_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    class B1HttpResponseAPIGenerator {
    public:
        B1HttpResponseAPIGenerator();
        virtual ~B1HttpResponseAPIGenerator();
    protected:
        virtual B1String implMakeResponse() = 0;
    public:
        B1String makeResponse();
    public:
        static std::unique_ptr<B1HttpResponseAPIGenerator> createAPIGenerator(const B1String& apiName);
    };
}   //  !BnD

#endif  // !_B1HTTP_HTTP_RESPONSE_API_GENERATOR_H
