//
// B1TimeChecker.h
//
// Library: B1Util
// Package: B1Util
// Module:  B1Util
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1UTIL_TIME_CHECKER_H
#define _B1UTIL_TIME_CHECKER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    class B1TimeChecker {
    public:
        B1TimeChecker();
    protected:
        bool _started;
        uint64 _interval;
        uint64 _lastTick;
    public:
        void start(uint64 interval);
        void restart();
        void stop();
        void reset();
        bool isRunning() const { return _started; }
        bool isTimeover(bool setIfTimeOver = true);
        uint64 interval() const { return _interval; }
        uint64 elapsed() const;
    };
}   //  !BnD

#endif  // !_B1UTIL_TIME_CHECKER_H
