//
// B1Condition.h
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1BASE_CONDITION_H
#define _B1BASE_CONDITION_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Base/B1Lock.h>

#if !defined(_WIN32)
#include <pthread.h>
#endif

namespace BnD {
    class B1Condition {
    public:
        B1Condition();
        virtual ~B1Condition();
    private:
#if defined(_WIN32)
        typedef void* Handle;
#else
        typedef pthread_cond_t Handle;
#endif
        Handle _handle;
        bool _signaled;
        B1Lock _lock;
    public:
        bool isSignaled() const { return _signaled; }
        bool wait(uint32 timeoutMillisec = 0);
        void signal();
        void reset();
    };
}   //  !BnD

#endif  // !_B1BASE_CONDITION_H
