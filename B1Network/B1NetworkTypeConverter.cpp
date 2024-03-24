//
// B1NetworkTypeConverter.cpp
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Network.h"
#include "B1NetworkTypeConverter.h"
#include "B1ClientSocket.h"
#include "B1ServerSocket.h"

using namespace BnD;

B1BaseSocket* B1NetworkTypeConverter::toBaseSocket(B1ClientSocket* handle)
{
    return static_cast<B1BaseSocket*>(handle);
}

B1BaseSocket* B1NetworkTypeConverter::toBaseSocket(B1ServerSocket* handle)
{
    return static_cast<B1BaseSocket*>(handle);
}

B1ClientSocket* B1NetworkTypeConverter::toClientSocket(B1BaseSocket* handle)
{
    return static_cast<B1ClientSocket*>(handle);
}

B1ServerSocket* B1NetworkTypeConverter::toServerSocket(B1BaseSocket* handle)
{
    return static_cast<B1ServerSocket*>(handle);
}
