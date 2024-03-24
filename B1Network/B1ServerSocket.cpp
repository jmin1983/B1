//
// B1ServerSocket.cpp
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Network.h"
#include "B1ServerSocket.h"

using namespace BnD;

B1ServerSocket::B1ServerSocket(const B1IOContext* context)
    : B1BaseSocket()
{
    resetSocket(context);
}

B1ServerSocket::~B1ServerSocket()
{
}
