//
// B1Condition.cpp
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Base.h"
#include "B1Condition.h"

#if defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#endif

using namespace BnD;

B1Condition::B1Condition()
    : _signaled(false)
    , _lock()
{
#if defined(_WIN32)
    _handle = ::CreateEvent(0, FALSE, FALSE, 0);
#else
    pthread_condattr_t cond_attr;
    ::pthread_condattr_init(&cond_attr);
    ::pthread_condattr_setclock(&cond_attr, CLOCK_MONOTONIC);
    ::pthread_cond_init(&_handle, &cond_attr);
#endif
}

B1Condition::~B1Condition()
{
#if defined(_WIN32)
    ::CloseHandle(_handle);
#else
    ::pthread_cond_destroy(&_handle);
#endif
}

bool B1Condition::wait(uint32 timeoutMillisec)
{
#if defined(_WIN32)
    _lock.lock();
    bool signaled = _signaled;
    _lock.unlock();
    if (signaled != true) {
        MSG msg;
        DWORD ret;
        DWORD waiting = (0 == timeoutMillisec) ? INFINITE : timeoutMillisec;
        do {
            if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
            ret = ::MsgWaitForMultipleObjects(1, &_handle, FALSE, waiting, QS_ALLINPUT);
        } while (ret != WAIT_OBJECT_0 && ret != WAIT_TIMEOUT);
    }
    _lock.lock();
    signaled = _signaled;   //  includes a signal arrived while waiting(or at the time of timed_out).
    _signaled = false;
    ::ResetEvent(_handle);
    _lock.unlock();
    return signaled;
#else
    _lock.lock();
    if (_signaled != true) {
        if (0 < timeoutMillisec) {
            struct timespec timeout;
            ::clock_gettime(CLOCK_MONOTONIC, &timeout);
            uint64 nsec = timeout.tv_nsec + static_cast<uint64>(timeout.tv_sec) * 1000000000 + static_cast<uint64>(timeoutMillisec) * 1000000;
            timeout.tv_sec = nsec / 1000000000;
            timeout.tv_nsec = nsec % 1000000000;
            while (_signaled != true) {
                if (::pthread_cond_timedwait(&_handle, &_lock.mutex(), &timeout) == ETIMEDOUT) {
                    break;
                }
            }
        }
        else {
            while (_signaled != true) {
                ::pthread_cond_wait(&_handle, &_lock.mutex());
            }
        }
    }
    const bool signaled = _signaled;    //  check and consume in the same lock. (spurious wakeup is ignored)
    _signaled = false;
    _lock.unlock();
    return signaled;
#endif
}

void B1Condition::signal()
{
    B1AutoLock al(_lock);

#if defined(_WIN32)
    ::SetEvent(_handle);
    _signaled = true;
#else
    if (!_signaled) {
        _signaled = true;
        ::pthread_cond_signal(&_handle);
    }
#endif
}

void B1Condition::reset()
{
    B1AutoLock al(_lock);

    _signaled = false;
#if defined(_WIN32)
    ::ResetEvent(_handle);
#endif
}
