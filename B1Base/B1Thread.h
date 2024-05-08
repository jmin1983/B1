//
// B1Thread.h
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1BASE_THREAD_H
#define _B1BASE_THREAD_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Base/B1Lock.h>

#if !defined(_WIN32)
#include <pthread.h>
#endif

namespace BnD {
    class B1Thread {
    public:
        B1Thread();
        virtual ~B1Thread();
        enum Status {
            STATUS_INVALID = -1,
            STATUS_READY = 0,
            STATUS_RUNNING,
            STATUS_END,
        };
        enum PRIORITY {
#if defined(_WIN32)
            PRIORITY_LOWEST = 1,
            PRIORITY_LOW = 25,
#endif
            PRIORITY_NORMAL = 50,
            PRIORITY_HIGH = 75,
            PRIORITY_HIGHEST = 99,
        };
    private:
        Status _status;
        B1Lock _statusLock;
    protected:
#if defined(_WIN32)
        typedef void* Handle;
#else
        typedef pthread_t Handle;
#endif
        Handle _handle;
    protected:
        void setStatus(Status status);
    protected:
        virtual void implThreadBegin() {}
        virtual void implThreadFunc() = 0;
        virtual void implThreadEnd() {}
    protected:
#if defined(_WIN32)
        static unsigned int __stdcall threadFunc(void* param);
#else
        static void* threadFunc(void* param);
#endif
    public:
        bool run();
        void join();
        void setPriority(PRIORITY value);
        bool isAlive() const;
        Status status() const { return _status; }
        static void sleep(uint32 millisecond);
        static void yield();
    };
}   //  !BnD

#endif  // !_B1BASE_THREAD_H
