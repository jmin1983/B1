//
// B1MainService.cpp
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Base.h"
#include "B1MainService.h"
#include "B1SystemUtil.h"
#include "B1TickUtil.h"
#include "B1Time.h"

using namespace BnD;

B1MainService::B1MainService(int32 version, B1String&& buildDate, B1String&& name)
    : _version(version)
    , _buildDate(buildDate)
    , _mainServiceName(name)
    , _mainServiceStatus(MAIN_SERVICE_STATUS_STOPPED)
    , _mainCondition()
{
}

B1MainService::~B1MainService()
{
}

std::map<B1String, B1String> B1MainService::makeVersionInfoMap() const
{
    std::map<B1String, B1String> values;
    values.insert(std::make_pair("BuildNum", softwareRev()));
    values.insert(std::make_pair("BuildDate", softwareDate()));
    values.insert(std::make_pair("StartTime", B1Time::currentTimeInMilliseconds(true)));
    return values;
}

bool B1MainService::start()
{
    if (implStart() != true) {
        implStop();
        return false;
    }
    _mainServiceStatus = MAIN_SERVICE_STATUS_WORKING;
    return true;
}

void B1MainService::wait()
{
    if (isStopped()) {
        return;
    }
    uint64 nextWaitInterval = CONSTS_MAIN_THREAD_PROCESS_INTERVAL_DEFAULT;
    while (MAIN_SERVICE_STATUS_WORKING == _mainServiceStatus) {
        if (_mainCondition.wait(nextWaitInterval) != true) {
            uint64 begin = B1TickUtil::currentTick();
            if (implWorking() != true) {
                _mainServiceStatus = MAIN_SERVICE_STATUS_STOPPING;
                stop();
            }
            else {
                uint64 diff = B1TickUtil::diffTick(begin, B1TickUtil::currentTick());
                if (CONSTS_MAIN_THREAD_PROCESS_INTERVAL_DEFAULT > diff) {
                    nextWaitInterval = std::max<uint64>(CONSTS_MAIN_THREAD_PROCESS_INTERVAL_MIN, CONSTS_MAIN_THREAD_PROCESS_INTERVAL_DEFAULT - diff);
                }
                else {
                    nextWaitInterval = CONSTS_MAIN_THREAD_PROCESS_INTERVAL_MIN;
                }
            }
        }
        else {
            _mainServiceStatus = MAIN_SERVICE_STATUS_STOPPING;
        }
    }
    implStop();
    _mainServiceStatus = MAIN_SERVICE_STATUS_STOPPED;
}

void B1MainService::stop()
{
    _mainCondition.signal();
}

B1String B1MainService::softwareRev() const
{
    return B1String::formatAs("%d", _version);
}

B1String B1MainService::softwareDate() const
{
    return _buildDate.copy();
}

B1String B1MainService::toString() const
{
    return B1String::formatAs("[%s] version:[%d][%s]", _mainServiceName.cString(), _version, _buildDate.cString());
}
