//
// GEGEMEventRepository.cpp
//
// Library: B1GEM
// Package: B1GEM
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1GEM.h"
#include "B1GEMEventRepository.h"

#include <B1SECS2/B1SECS2Data.h>

using namespace BnD;

B1GEMEventRepository::B1GEMEventRepository()
    : _lock()
    , _events()
{
}

B1GEMEventRepository::~B1GEMEventRepository()
{
}

bool B1GEMEventRepository::isGetAllRequested(const std::vector<B1SECS2DataCEID>& ceIDs) const
{
    return ceIDs.empty();
}

bool B1GEMEventRepository::initialize(const std::set<B1GEMEvent>& events)
{
    _events.clear();
    for (const auto& evt : events) {
        _events.insert(std::make_pair(evt.ceID(), std::make_shared<B1GEMEvent>(evt)));
    }
    return true;
}

void B1GEMEventRepository::finalize()
{
    _events.clear();
}

void B1GEMEventRepository::deleteAllReports()
{
    B1AutoLock al(_lock);
    for (const auto& eventsPair : _events) {
        eventsPair.second->reportIDs().clear();
    }
}

B1GEMEventRepository::RESULT B1GEMEventRepository::linkReport(const std::map<B1SECS2DataCEID, std::list<std::shared_ptr<B1SECS2DataRPTID> > >& linkData)
{
    B1AutoLock al(_lock);
    std::map<const B1SECS2DataCEID, SPGEMEvent> events;
    for (const auto& eventsPair : _events) {
        events.insert(std::make_pair(eventsPair.first, eventsPair.second->clone()));
    }
    for (const auto& linkDataPair : linkData) {
        auto itrEvent = events.find(linkDataPair.first);
        if (itrEvent == events.end()) {
            B1LOG("linkReport failed. no CEID found: [%s]", linkDataPair.first->toString().cString());
            assert(false);
            return RESULT_AT_LEAST_ONE_CEID_DOES_NOT_EXIST;
        }
        if (linkDataPair.second.empty()) {
            itrEvent->second->reportIDs().clear();
        }
        else {
            if (itrEvent->second->reportIDs().empty() != true) {
                B1LOG("LinkReport failed. Already defined: [%s]", linkDataPair.first->toString().cString());
                assert(false);
                return RESULT_AT_LEAST_ONE_CEID_LINK_IS_ALREADY_DEFINED;
            }
            itrEvent->second->reportIDs().insert(itrEvent->second->reportIDs().end(), linkDataPair.second.begin(), linkDataPair.second.end());
        }
    }
    events.swap(_events);
    return RESULT_ACCEPTED;
}

bool B1GEMEventRepository::getAvailableCEIDs(const std::vector<B1SECS2DataCEID>& ceIDs, std::vector<B1SECS2DataCEID>* result) const
{
    bool getAllRequested = isGetAllRequested(ceIDs);
    B1AutoLock al(_lock);
    if (getAllRequested) {
        result->reserve(_events.size());
        for (const auto& eventsPair : _events) {
            result->push_back(eventsPair.first);
        }
    }
    else {
        result->reserve(ceIDs.size());
        for (const auto& ceID : ceIDs) {
            auto itr = _events.find(ceID);
            if (itr != _events.end()) {
                result->push_back(itr->first);
            }
            else {
                result->clear();
                B1LOG("No CEID found: [%s]", ceID->toString().cString());
                assert(false);
                return false;
            }
        }
    }
    return true;
}

SPGEMEvent B1GEMEventRepository::findEvent(const B1SECS2DataCEID& ceID) const
{
    B1AutoLock al(_lock);
    auto itr = _events.find(ceID);
    return itr != _events.end() ? itr->second : SPGEMEvent();
}
