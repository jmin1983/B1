//
// B1Network.h
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1NETWORK_H
#define _B1NETWORK_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Base/B1Base.h>  //  include first
#include <B1Base/B1String.h>
#include <B1Base/B1Logger.h>

#if defined(_WIN32) && !defined(_WIN32_WINNT)
#define _WIN32_WINNT 0x0A00
#endif
#include <boost/asio.hpp>

#endif  // !_B1NETWORK_H
