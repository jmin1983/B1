//
// B1DaemonService.h
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1BASE_DAEMON_SERVICE_H
#define _B1BASE_DAEMON_SERVICE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Base/B1MainService.h>
#include <B1Base/B1DaemonDelegate.h>

namespace BnD {
    class B1DaemonService : protected B1DaemonDelegate
                          , public B1MainService {
    public:
        B1DaemonService(int32 serviceID, int32 version, B1String&& buildDate, B1String&& serviceName, B1String&& systemName);
        virtual ~B1DaemonService();
    private:
        std::shared_ptr<class B1Daemon> _daemon;
        bool _stopService;
    protected:
        virtual int startWorkingProcess() override;
    protected:
        virtual bool needWorkingProcessOn() = 0;
    public:
        bool startDaemon(uint32 restartInterval = 0);   //  return true if work_process(child)
        void stopDaemon();
        bool startService();
        void stopService();
    };
}   //  !BnD

#endif  // !_B1BASE_DAEMON_SERVICE_H
