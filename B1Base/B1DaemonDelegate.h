//
// B1DaemonDelegate.h
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1BASE_DAEMON_DELEGATE_H
#define _B1BASE_DAEMON_DELEGATE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    class B1DaemonDelegate {
    public:
        virtual int startWorkingProcess() { return 0; } //  return 0: work process, return < 0: error or no_need_to process, return > 0: daemon pid
    };
}   //  !BnD

#endif  // !_B1BASE_DAEMON_DELEGATE_H
