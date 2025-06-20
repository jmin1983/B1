//
// B1Lock.h
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1BASE_LOCK_H
#define _B1BASE_LOCK_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Base/B1Mutex.h>

namespace BnD {
    class B1Lockable {
    public:
        virtual ~B1Lockable() {}
        virtual void lock() = 0;
        virtual void unlock() = 0;
    };

    template<typename T>
    class B1BaseLock : public B1Lockable {
    public:
        B1BaseLock() {}
        B1BaseLock(const B1BaseLock&) = delete;
        B1BaseLock& operator=(const B1BaseLock& s) = delete;
    protected:
        mutable T _mutex;
    public:
        bool tryLock() { return _mutex.try_lock(); }
        virtual void lock() override { _mutex.lock(); }
        virtual void unlock() override { _mutex.unlock(); }
#if !defined(_WIN32)
        pthread_mutex_t& mutex() { return _mutex.nativeHandle(); }
#endif
    };

    class B1Lock : public B1BaseLock<B1Mutex> {};
    class B1RecursiveLock : public B1BaseLock<B1RecursiveMutex> {};

    class B1AutoLock {
    public:
        explicit B1AutoLock(B1Lockable& lock) : _lock(lock) { _lock.lock(); }
        B1AutoLock(const B1AutoLock&) = delete;
        ~B1AutoLock() { _lock.unlock(); }
        B1AutoLock& operator=(const B1AutoLock& s) = delete;
    private:
        B1Lockable& _lock;
    };
}   //  !BnD

#endif  // !_B1BASE_LOCK_H
