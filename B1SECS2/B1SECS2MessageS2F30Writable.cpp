//
// B1SECS2MessageS2F30Writable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F30Writable.h"

using namespace BnD;

B1SECS2MessageS2F30Writable::B1SECS2MessageS2F30Writable(const std::vector<B1SECS2DataECID>& ecIDs,
                                                         const std::vector<B1SECS2DataECNAME>& ecNames,
                                                         const std::vector<B1SECS2DataECMIN>& ecMins,
                                                         const std::vector<B1SECS2DataECMAX>& ecMaxes,
                                                         const std::vector<B1SECS2DataECDEF>& ecDefs,
                                                         const std::vector<B1SECS2DataUNITS>& units)
    : B1SECS2MessageWritable(false, 2, 30, "sendMessageS2F30")
    , _ecIDs(ecIDs)
    , _ecNames(ecNames)
    , _ecMins(ecMins)
    , _ecMaxes(ecMaxes)
    , _ecDefs(ecDefs)
    , _units(units)
{
}

B1SECS2MessageS2F30Writable::~B1SECS2MessageS2F30Writable()
{
}

B1String B1SECS2MessageS2F30Writable::fullName()
{
    return "Equipment Constant Namelist (ECN)";
}

bool B1SECS2MessageS2F30Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Equipment Constant Namelist (ECN)
    <L n
        <L 6
            <ECID>
            <ECNAME>
            <ECMIN>
            <ECMAX>
            <ECDEF>
            <UNITS>
        >
    >
    */
    size_t count = _ecIDs.size();
    count = std::min(count, _ecNames.size());
    count = std::min(count, _ecMins.size());
    count = std::min(count, _ecMaxes.size());
    count = std::min(count, _ecDefs.size());
    count = std::min(count, _units.size());
    addList(count, data);
    for (size_t i = 0; i < count; ++i) {
        addList(6, data);
        addData(_ecIDs[i], data);
        addData(_ecNames[i], data);
        addData(_ecMins[i], data);
        addData(_ecMaxes[i], data);
        addData(_ecDefs[i], data);
        addData(_units[i], data);
    }
    return true;
}
