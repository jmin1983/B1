//
// B1DaemonService.cpp
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Base.h"
#include "B1DaemonService.h"
#include "B1Daemon.h"
#include "B1Thread.h"

#if !defined(_WIN32)
#include <unistd.h>
#endif

using namespace BnD;

B1DaemonService::B1DaemonService(int32 serviceID, int32 version, B1String&& buildDate, B1String&& serviceName, B1String&& systemName)
    : B1DaemonDelegate()
    , B1MainService(serviceID, version, std::move(buildDate), std::move(serviceName), std::move(systemName))
    , _daemon()
    , _stopService(false)
{
}

B1DaemonService::~B1DaemonService()
{
}

int B1DaemonService::startWorkingProcess()
{
    if (needWorkingProcessOn() != true)
        return -1;
#if defined(_WIN32)
    return 0;
#else
    return fork();
#endif
}

bool B1DaemonService::startDaemon(uint32 restartInterval)
{
    _daemon.reset(new B1Daemon(restartInterval));
    if (_daemon->startDaemon(this) != true)
        return false;
    _daemon.reset();
    return true;
}

void B1DaemonService::stopDaemon()
{
    if (_daemon) {
        _daemon->setStopDaemon();
    }
}

bool B1DaemonService::startService()
{
    while (_stopService != true) {
        if (needWorkingProcessOn()) {
            break;
        }
        B1Thread::sleep(1000);
    }
    if (_stopService) {
        exit(0);
    }
    return true;
}

void B1DaemonService::stopService()
{
    _stopService = true;
}
