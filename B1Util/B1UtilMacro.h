//
// B1UtilMacro.h
//
// Library: B1Util
// Package: B1Util
// Module:  B1Util
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1UTIL_MACRO_H
#define _B1UTIL_MACRO_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/typeof/typeof.hpp>

#define SAFE_CONTAINER_FOREACH(c,i) for(BOOST_TYPEOF((c).begin()) end##i = (c).end(), next##i = (c).begin(), \
                                   i = (next##i==end##i)?next##i:next##i++; \
                                   i != next##i; \
                                   i = (next##i==end##i)?next##i:next##i++)

#endif  // !_B1UTIL_MACRO_H
