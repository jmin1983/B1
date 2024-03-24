//
// B1Mutex.h
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1BASE_MUTEX_H
#define _B1BASE_MUTEX_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <mutex>

namespace BnD {
#if defined(_WIN32)
    class B1Mutex : public std::mutex {};
    class B1RecursiveMutex : public std::recursive_mutex {};
#else
    class B1Mutex_linux {
    public:
        B1Mutex_linux(bool recursive);
        virtual ~B1Mutex_linux();
    protected:
        pthread_mutex_t	_mutex;
    public:
        bool try_lock();
        void lock();
        void unlock();
        pthread_mutex_t& nativeHandle() { return _mutex; }
    };

    class B1Mutex : public B1Mutex_linux {
    public:
        B1Mutex() : B1Mutex_linux(false) {}
    };

    class B1RecursiveMutex : public B1Mutex_linux {
    public:
        B1RecursiveMutex() : B1Mutex_linux(true) {}
    };
#endif
}   //  !BnD

#endif  // !_B1BASE_MUTEX_H
