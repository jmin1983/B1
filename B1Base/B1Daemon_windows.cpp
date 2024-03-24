//
// B1Daemon_windows.cpp
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Base.h"

#if defined(_WIN32)

#include "B1DaemonDelegate.h"
#include "B1String.h"
#include "B1Thread.h"

using namespace BnD;

bool B1Daemon::startDaemon(B1DaemonDelegate* delegate)
{
    int pid = 0;
    while (_stopDaemon != true) {
        if ((pid = delegate->startWorkingProcess()) < 0) {
            B1Thread::sleep(1000);
        }
        else if (pid == 0) {
            return true;
        }
    }
    return true;
}

#endif
