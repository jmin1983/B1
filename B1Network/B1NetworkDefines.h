//
// B1NetworkDefines.h
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1NETWORK_DEFINES_H
#define _B1NETWORK_DEFINES_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(_WIN32)
#undef FD_SETSIZE
#define FD_SETSIZE  1024
#include <WinSock2.h>
typedef SOCKET SocketHandle;
#else
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
typedef int SocketHandle;
#endif  // _WIN32

#endif  // !_B1NETWORK_DEFINES_H
