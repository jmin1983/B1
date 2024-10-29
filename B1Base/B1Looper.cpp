//
// B1Looper.cpp
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Base.h"
#include "B1Looper.h"
#include "B1TickUtil.h"

using namespace BnD;

B1Looper::B1Looper()
    : B1Thread()
    , _event()
    , _resumeInterval(0)
    , _quitRequested(false)
{
}

B1Looper::~B1Looper()
{
}

void B1Looper::implThreadFunc()
{
    while (_quitRequested != true) {
        uint64 resumeInterval = _resumeInterval;
        uint64 begin = 0;
        if (resumeInterval > 0) {
            begin = B1TickUtil::currentTick();
        }
        implLooperFunc();
        if (resumeInterval > 0) {
            uint64 diff = B1TickUtil::diffTick(begin, B1TickUtil::currentTick());
            uint64 interval = resumeInterval > diff ? resumeInterval - diff : 0;
            if (interval) {
                _event.wait(static_cast<uint32>(interval));
            }
        }
    }
}

void B1Looper::implThreadEnd()
{
    _quitRequested = false;
    _event.signal();
}
