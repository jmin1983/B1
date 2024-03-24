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
    bool signaled = false;
    _lock.lock();
    if (isSignaled()) {
        _lock.unlock();
        signaled = true;
    }
    else {
#if defined(_WIN32)
        _lock.unlock();
        MSG msg;
        DWORD ret;
        DWORD waiting = (0 == timeoutMillisec) ? INFINITE : timeoutMillisec;
        do {
            if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
            ret = ::MsgWaitForMultipleObjects(1, &_handle,
                                              FALSE, waiting, QS_ALLINPUT);
        } while (ret != WAIT_OBJECT_0 && ret != WAIT_TIMEOUT);
        signaled = (ret == WAIT_OBJECT_0);
#else
        int32 ret = 0;
        if (0 < timeoutMillisec) {
            struct timespec timeout;
            ::clock_gettime(CLOCK_MONOTONIC, &timeout);
            uint64 nsec = timeout.tv_nsec + static_cast<uint64>(timeout.tv_sec) * 1000000000 + static_cast<uint64>(timeoutMillisec) * 1000000;
            timeout.tv_sec = nsec / 1000000000;
            timeout.tv_nsec = nsec % 1000000000;
            ret = ::pthread_cond_timedwait(&_handle, &_lock.mutex(), &timeout);
        }
        else {
            ret = ::pthread_cond_wait(&_handle, &_lock.mutex());
        }
        _lock.unlock();
        signaled = (ret == 0);
#endif
    }

    reset();
    return signaled;
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
