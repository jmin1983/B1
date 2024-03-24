//
// B1TimeChecker.cpp
//
// Library: B1Util
// Package: B1Util
// Module:  B1Util
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Util.h"
#include "B1TimeChecker.h"

#include <B1Base/B1TickUtil.h>

using namespace BnD;

B1TimeChecker::B1TimeChecker()
    : _started(false)
    , _interval(0)
    , _lastTick(0)
{
}

void B1TimeChecker::start(uint64 interval)
{
    _interval = interval;
    _lastTick = B1TickUtil::currentTick();
    _started = true;
}

void B1TimeChecker::restart()
{
    _lastTick = B1TickUtil::currentTick();
    _started = true;
}

void B1TimeChecker::stop()
{
    if (_started) {
        _started = false;
    }
}

void B1TimeChecker::reset()
{
    if (_started) {
        _lastTick = B1TickUtil::currentTick();
    }
}

bool B1TimeChecker::isTimeover(bool setIfTimeOver)
{
    if (_started != true) {
        return false;
    }
    uint64 now = B1TickUtil::currentTick();
    if (B1TickUtil::diffTick(_lastTick, now) < _interval) {
        return false;
    }
    if (setIfTimeOver) {
        _lastTick = now;
    }
    return true;
}

uint64 B1TimeChecker::elapsed() const
{
    if (_started != true) {
        return 0;
    }
    return B1TickUtil::currentTick() - _lastTick;
}