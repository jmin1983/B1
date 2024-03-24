//
// B1WebSocketMessage.cpp
//
// Library: B1Http
// Package: B1Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Http.h"
#include "B1WebSocketMessage.h"

using namespace BnD;

B1WebSocketMessage::B1WebSocketMessage()
    : _buffer()
{
}

B1WebSocketMessage::~B1WebSocketMessage()
{
}

void B1WebSocketMessage::clearRequest()
{
    _buffer.consume(_buffer.size());
}
