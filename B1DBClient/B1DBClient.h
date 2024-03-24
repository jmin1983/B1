//
// B1DBClient.h
//
// Library: B1DBClient
// Package: B1DBClient
// Module:  B1DBClient
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1DBCLIENT_H
#define _B1DBCLIENT_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Base/B1Base.h>  //  include first
#include <B1Base/B1String.h>
#include <B1Base/B1Logger.h>

#if defined(_WIN32) && !defined(_WIN32_WINNT)
#define _WIN32_WINNT 0x0A00
#endif
#include <boost/mysql.hpp>

#endif  // !_B1DBCLIENT_H
