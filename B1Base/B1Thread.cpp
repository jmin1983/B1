//
// B1Thread.cpp
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Base.h"
#include "B1Thread.h"

#if defined(_WIN32)
#include <process.h>
#include <Windows.h>
#else
#include <time.h>
#endif

using namespace BnD;

B1Thread::B1Thread()
    : _status(STATUS_INVALID)
    , _statusLock()
    , _handle(0)
{
}

B1Thread::~B1Thread()
{
}

void B1Thread::setStatus(Status status)
{
    B1AutoLock al(_statusLock);
    _status = status;
}

#if defined(_WIN32)
unsigned int __stdcall B1Thread::threadFunc(void* param)
#else
void* B1Thread::threadFunc(void* param)
#endif
{
    B1Thread* thread = reinterpret_cast<B1Thread*>(param);
    thread->setStatus(STATUS_RUNNING);
#if defined(_WIN32) && !defined(_DEBUG)
    __try {
#endif
        thread->implThreadBegin();
        thread->implThreadFunc();
        thread->implThreadEnd();
#if defined(_WIN32) && !defined(_DEBUG)
    }
    __except (B1DUMP(GetExceptionInformation(), typeid(*thread).name(), 6)) {
        abort();
    }
#endif
    thread->setStatus(STATUS_END);
    return 0;
}

bool B1Thread::run()
{
    assert(0 == _handle);
#if defined(_WIN32)
    _handle = (void*)::_beginthreadex(0, 0, threadFunc, this, 0, NULL);
#else
    int32 errorCode = ::pthread_create(&_handle, 0, threadFunc, this);
    if (errorCode != 0) {
        _handle = 0;
    }
#endif
    if (0 == _handle) {
        return false;
    }
    setStatus(STATUS_READY);
    return true;
}

void B1Thread::join()
{
    if (0 == _handle) {
        return;
    }
#if defined(_WIN32)
    while (isAlive()) {
        B1Thread::sleep(10);
    }
    CloseHandle(_handle);
#else   // pthread
    void* p = 0;
    ::pthread_join(_handle, &p);
#endif
    _handle = 0;
    setStatus(STATUS_INVALID);
}

void B1Thread::setPriority(PRIORITY value)
{
    if (0 == _handle)
        return;
    if (isAlive() != true)
        return;
#if defined (_WIN32)
    int priority = 0;
    switch (value) {
        case PRIORITY_LOWEST:
            priority = THREAD_PRIORITY_LOWEST;
            break;
        case PRIORITY_LOW:
            priority = THREAD_PRIORITY_BELOW_NORMAL;
            break;
        default:
        case PRIORITY_NORMAL:
            priority = THREAD_PRIORITY_NORMAL;
            break;
        case PRIORITY_HIGH:
            priority = THREAD_PRIORITY_ABOVE_NORMAL;
            break;
        case PRIORITY_HIGHEST:
            //priority = THREAD_PRIORITY_HIGHEST;
            priority = THREAD_PRIORITY_TIME_CRITICAL;
            break;
    }
    ::SetThreadPriority(_handle, priority);
#else
    int policy;
    struct sched_param param;
    ::pthread_getschedparam(_handle, &policy, &param);
    if (PRIORITY_NORMAL == value) {
        param.sched_priority = 0;
        policy = SCHED_OTHER;
    }
    else {
        param.sched_priority = value;
        policy = SCHED_FIFO;
    }
    ::pthread_setschedparam(_handle, policy, &param);
#endif
}

uint32 B1Thread::threadId() const
{
#if defined(_WIN32)
    return ::GetCurrentThreadId();
#else
    return ::pthread_self();
#endif
}

bool B1Thread::isAlive() const
{
    return STATUS_READY ==      _status ||
           STATUS_RUNNING ==    _status;   //  locking?
}

void B1Thread::sleep(uint32 millisecond)
{
#if defined(_WIN32)
    ::Sleep(millisecond);
#else
    struct timespec ts_sleep, ts_remaining;
    ts_sleep.tv_sec = millisecond / 1000;
    ts_sleep.tv_nsec = (millisecond % 1000) * 1000000;
    ::nanosleep(&ts_sleep, &ts_remaining);
#endif
}

void B1Thread::yield()
{
#if defined(_WIN32)
    ::SwitchToThread();
#else
    ::sched_yield();
#endif
}
