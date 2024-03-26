//
// B1GEMEvent.cpp
//
// Library: B1GEM
// Package: B1GEM
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1GEM.h"
#include "B1GEMEvent.h"

using namespace BnD;

B1GEMEvent::B1GEMEvent(const B1GEMEvent& evt, bool deepCopy)
    : _ceID(static_cast<const B1SECS2DataCEID*>(evt._ceID->clone()))
    , _ceName(static_cast<const B1SECS2DataCENAME*>(evt._ceName->clone()))
    , _reportIDs()
{
    for (auto reportID : evt._reportIDs) {
        _reportIDs.push_back(std::shared_ptr<B1SECS2DataRPTID>(static_cast<B1SECS2DataRPTID*>(reportID->clone())));
    }
}

B1GEMEvent::B1GEMEvent(const B1SECS2DataCEID& ceID, const B1SECS2DataCENAME& ceName)
    : _ceID(std::make_shared<const B1SECS2DataCEID>(ceID))
    , _ceName(std::make_shared<const B1SECS2DataCENAME>(ceName))
    , _reportIDs()
{
}

B1GEMEvent::~B1GEMEvent()
{
}
