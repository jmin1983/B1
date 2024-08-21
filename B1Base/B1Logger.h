//
// B1Logger.h
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1BASE_LOGGER_H
#define _B1BASE_LOGGER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/core/demangle.hpp>

#if defined(_WIN32)
#define __CLASS_NAME__ boost::core::demangle(typeid(*this).name()).substr(6)
#else
#define __CLASS_NAME__ boost::core::demangle(typeid(*this).name())
#endif
#define B1LOG(...) b1log_cl((__CLASS_NAME__), __VA_ARGS__);

namespace BnD {
    typedef bool(*b1log_callback_t)(const char* log, void* param);
    class B1FileLog;
    class B1String;
    B1String b1log(const char* format, ...);
    B1String b1log_cl(const std::string& className, const char* format, ...);
    bool testB1FileLog();
    void b1log(const B1String& string);
    void setLogger(B1FileLog* fileLogger);
    void setLoggerCallback(b1log_callback_t callback, void* param = NULL);
    void resetLogCallback();
}   //  !BnD

#endif  // !_B1BASE_LOGGER_H
