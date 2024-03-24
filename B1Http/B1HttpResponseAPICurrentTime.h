//
// B1HttpResponseAPICurrentTime.h
//
// Library: B1Http
// Package: B1Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1HTTP_HTTP_RESPONSE_API_CURRENT_TIME_H
#define _B1HTTP_HTTP_RESPONSE_API_CURRENT_TIME_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Http/B1HttpResponseAPIGenerator.h>

namespace BnD {
    class B1HttpResponseAPICurrentTime : public B1HttpResponseAPIGenerator {
    protected:
        B1String implMakeResponse() final;
    };
}   //  !BnD

#endif  // !_B1HTTP_HTTP_RESPONSE_API_CURRENT_TIME_H
