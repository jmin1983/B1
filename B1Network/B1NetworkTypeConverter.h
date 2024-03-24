//
// B1NetworkTypeConverter.h
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1NETWORK_TYPE_CONVERTER_H
#define _B1NETWORK_TYPE_CONVERTER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    class B1BaseSocket;
    class B1ClientSocket;
    class B1ServerSocket;
    namespace B1NetworkTypeConverter {
        B1BaseSocket* toBaseSocket(B1ClientSocket* handle);
        B1BaseSocket* toBaseSocket(B1ServerSocket* handle);
        B1ClientSocket* toClientSocket(B1BaseSocket* handle);
        B1ServerSocket* toServerSocket(B1BaseSocket* handle);
    };
}   //  !BnD

#endif  // !_B1NETWORK_TYPE_CONVERTER_H
