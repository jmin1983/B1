//
// B1IOLooper.cpp
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Network.h"
#include "B1IOLooper.h"
#include "B1BaseSessionManager.h"
#include "B1IOContext.h"

using namespace BnD;

B1IOLooper::B1IOLooper()
    : B1Looper()
    , _ioContext()
{
}

B1IOLooper::~B1IOLooper()
{
}

void B1IOLooper::implLooperFunc()
{
    _ioContext->runNativeContext();
    B1Thread::sleep(1);
}

bool B1IOLooper::startup()
{
    _ioContext.reset(new B1IOContext(CONSTS_THREAD_COUNT_HINT));
    if (implStartup() != true) {
        _ioContext.reset();
        return false;
    }
    return run();
}

void B1IOLooper::shutdown()
{
    implShutdown();
    quit();
    join();
    if (_ioContext) {
        //_ioContext->stop();   //  If it didn't stop, then something went wrong(it means _ioContext is still waiting some tasks).
        assert(_ioContext->isStopped());
        _ioContext.reset();
    }
}

bool B1IOLooper::pause()
{
    quit();
    _ioContext->stopNativeContext();
    join();
    for (int32 i = 0; i < 500; ++i) {
        while (_ioContext->isNativeContextStopped()) {
            return true;
        }
        B1Thread::sleep(10);
    }
    B1LOG("B1IOLooper pause failed");
    return false;
}

bool B1IOLooper::resume()
{
    if (isAlive()) {
        B1LOG("B1IOLooper resume failed. already running");
        return false;
    }
    _ioContext->restartNativeContext();
    if (run() != true) {
        B1LOG("B1IOLooper resume failed");
        return false;
    }
    return true;
}
