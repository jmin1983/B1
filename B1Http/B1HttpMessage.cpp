//
// B1HttpMessage.cpp
//
// Library: B1Http
// Package: B1Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Http.h"
#include "B1HttpMessage.h"

using namespace BnD;

B1HttpMessage::B1HttpMessage()
    : _buffer()
    , _request()
{
}

B1HttpMessage::~B1HttpMessage()
{
}

void B1HttpMessage::clearRequest()
{
    _request = {};
}
