//
// B1Daemon.h
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1BASE_DAEMON_H
#define _B1BASE_DAEMON_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    class B1Daemon {
    public:
        B1Daemon(uint32 restartInterval) : _stopDaemon(false), _restartInterval(restartInterval) {}
    protected:
        bool _stopDaemon;
        uint32 _restartInterval;
    protected:
        void writeError(const char* file, const char* msg, int code);
    public:
        bool startDaemon(class B1DaemonDelegate* delegate); //  return true if work_process(child)
        void setStopDaemon() { _stopDaemon = true; }
    };
}   //  !BnD

#endif  // !_B1BASE_DAEMON_H
