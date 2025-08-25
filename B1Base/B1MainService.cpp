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
#include "B1SystemUtil.h"

using namespace BnD;

B1MainService::B1MainService(int32 serviceID, int32 version, B1String&& buildDate, B1String&& serviceName, B1String&& systemName)
    : _serviceID(serviceID)
    , _version(version)
    , _buildDate(buildDate)
    , _mainServiceName(serviceName)
    , _systemServiceName(systemName)
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
    values.insert(std::make_pair("ServiceName", _mainServiceName.copy()));
    values.insert(std::make_pair("SystemName", _systemServiceName.copy()));
    values.insert(std::make_pair("ServiceID", B1String::formatAs("%d", serviceID())));
    values.insert(std::make_pair("SystemID", B1SystemUtil::getSystemID()));
    values.insert(std::make_pair("BuildNum", softwareRev()));
    values.insert(std::make_pair("BuildDate", softwareDate()));
    values.insert(std::make_pair("StartTime", B1Time::currentTimeInMilliseconds(true)));
    B1String addressesString = getNetworkAddressesString();
    if (addressesString.isEmpty() != true) {
        values.insert(std::make_pair("Addresses", addressesString));
    }
    return values;
}

B1String B1MainService::getNetworkAddressesString() const
{
    B1String addressesString;
    std::list<B1String> addresses;
    B1SystemUtil::getLocalNetworkAddresses(&addresses);
    for (const auto& address : addresses) {
        if (addressesString.isEmpty() != true) {
            addressesString.append(" / ");
        }
        addressesString += address;
    }
    return addressesString;
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
    uint32 nextWaitInterval = CONSTS_MAIN_THREAD_PROCESS_INTERVAL_DEFAULT;
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
                    nextWaitInterval = std::max<uint32>(CONSTS_MAIN_THREAD_PROCESS_INTERVAL_MIN, static_cast<uint32>(CONSTS_MAIN_THREAD_PROCESS_INTERVAL_DEFAULT - diff));
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
    return B1String::formatAs("[%s][%d] version:[%d][%s]", _mainServiceName.cString(), _serviceID, _version, _buildDate.cString());
}
