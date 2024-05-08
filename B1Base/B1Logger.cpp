//
// B1Logger.cpp
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Base.h"
#include "B1Logger.h"
#include "B1FileLog.h"
#include "B1Lock.h"
#include "B1SystemUtil.h"
#include "B1Time.h"

#include <stdarg.h>
#include <stdio.h>

#if defined(_WIN32) && defined(_DEBUG)
#include <Windows.h>
#endif

BnD::B1String BnD::b1log(const char* format, ...)
{
    B1String s;
    va_list args;
    va_start(args, format);
    s.appendvf(format, args);
    va_end(args);
    b1log(s);
    return s;
}

BnD::B1String BnD::b1log_cl(const std::string &className, const char* format, ...)
{
    B1String s = className + ": ";
    va_list args;
    va_start(args, format);
    s.appendvf(format, args);
    va_end(args);
    b1log(s);
    return s;
}

BnD::b1log_callback_t g_callback = NULL;
void* g_callbackParam = NULL;
BnD::B1FileLog* g_fileLogger = NULL;

bool BnD::testV1FileLog()
{
    return g_fileLogger && g_fileLogger->testWriteLogFile();
}

void BnD::b1log(const B1String &string)
{
    B1String timedLog;
    timedLog.format("[%s][%u] %s\n", B1Time::currentTimeInMicroseconds().cString(), B1SystemUtil::getCurrentThreadID(), string.cString());
    if (g_fileLogger) {
        g_fileLogger->write(g_callback ? timedLog.copy() : std::move(timedLog));
    }
    else {
        static BnD::B1Lock s_log_lock;
        B1AutoLock al(s_log_lock);
        printf("%s", timedLog.cString());
#if defined(_WIN32) && defined(_DEBUG)
        OutputDebugStringA(timedLog.cString());
#endif
    }
    if (g_callback) {
        g_callback(timedLog.cString(), g_callbackParam);
    }
}

void BnD::setLogger(B1FileLog *fileLogger)
{
    g_fileLogger = fileLogger;
}

void BnD::setLoggerCallback(b1log_callback_t callback, void* param)
{
    g_callback = callback;
    g_callbackParam = param;
}

void BnD::resetLogCallback()
{
    g_callback = NULL;
    g_callbackParam = NULL;
}