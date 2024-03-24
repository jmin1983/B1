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

#if defined(_DO_NOT_USE_B1LOG)
#define B1LOG(...) ""
#else
#define __CLASS_NAME__ std::string(typeid(*this).name()).substr(6)
#define B1LOG(...) v2log_cl((__CLASS_NAME__), __VA_ARGS__);
#endif

#include <string>

namespace BnD {
    typedef bool(*v2log_callback_t)(const char* log, void* param);
    class B1FileLog;
    class B1String;
    B1String v2log(const char* format, ...);
    B1String v2log_cl(const std::string& className, const char* format, ...);
    bool testV1FileLog();
    void v2log(const B1String& string);
    void setLogger(B1FileLog *fileLogger);
    void setLoggerCallback(v2log_callback_t callback, void* param = NULL);
    void resetLogCallback();
}   //  !BnD

#endif  // !_B1BASE_LOGGER_H
