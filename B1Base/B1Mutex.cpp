//
// B1Mutex.cpp
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Base.h"
#include "B1Mutex.h"

using namespace BnD;

#if !defined(_WIN32)

B1Mutex_linux::B1Mutex_linux(bool recursive)
{
    if (recursive) {
        pthread_mutexattr_t attribute;
        pthread_mutexattr_init(&attribute);
        pthread_mutexattr_settype(&attribute, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&_mutex, &attribute);
    }
    else {
        pthread_mutex_init(&_mutex, 0);
    }
}

B1Mutex_linux::~B1Mutex_linux()
{
    pthread_mutex_destroy(&_mutex);
}

bool B1Mutex_linux::try_lock()
{
    return pthread_mutex_trylock(&_mutex) == 0;
}

void B1Mutex_linux::lock()
{
    pthread_mutex_lock(&_mutex);
}

void B1Mutex_linux::unlock()
{
    pthread_mutex_unlock(&_mutex);
}

#endif
