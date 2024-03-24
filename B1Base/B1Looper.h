//
// B1Looper.h
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1BASE_LOOPER_H
#define _B1BASE_LOOPER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Base/B1Condition.h>
#include <B1Base/B1Thread.h>

namespace BnD {
    class B1Looper : public B1Thread {
    public:
        B1Looper();
        virtual ~B1Looper();
    private:
        B1Condition _event;
        uint64 _resumeInterval;
        bool _quitRequested;
    protected:
        virtual void implThreadFunc() override;
        virtual void implThreadEnd() override;
    protected:
        virtual void implLooperFunc() = 0;
    public:
        void setResumeInterval(uint64 interval) { _resumeInterval = interval; } //  see timeBeginPeriod() for Windows.
        void quit() { _quitRequested = true; _event.signal(); }
        void reset() { _quitRequested = false; }
        bool isQuitRequested() const { return _quitRequested; }
    };
}   //  !BnD

#endif  // !_B1BASE_LOOPER_H
