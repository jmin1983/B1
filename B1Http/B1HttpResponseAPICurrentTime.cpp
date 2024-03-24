//
// B1HttpResponseAPICurrentTime.cpp
//
// Library: B1Http
// Package: B1Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Http.h"
#include "B1HttpResponseAPICurrentTime.h"

#include <B1Base/B1Time.h>

using namespace BnD;

B1String B1HttpResponseAPICurrentTime::implMakeResponse()
{
    return B1Time::currentTimeInMicroseconds();
}
