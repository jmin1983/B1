//
// B1SECS2MessageS3F17Writable.cpp
//
// Library: B1SECS2
// Package: S3
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS3F17Writable.h"

using namespace BnD;

B1SECS2MessageS3F17Writable::B1SECS2MessageS3F17Writable(const B1SECS2DataDATAID& dataID,
                                                         const B1SECS2DataCARRIERACTION& carrierAction,
                                                         const B1SECS2DataCARRIERID& carrierID,
                                                         const B1SECS2DataPTN& ptn,
                                                         const std::map<B1SECS2DataCATTRID, B1SECS2DataCATTRDATA>& cattrData)
    : B1SECS2MessageWritable(true, 3, 17, "sendMessageS3F17")
    , _dataID(dataID)
    , _carrierAction(carrierAction)
    , _carrierID(carrierID)
    , _ptn(ptn)
    , _cattrData(cattrData)
{
}

B1SECS2MessageS3F17Writable::~B1SECS2MessageS3F17Writable()
{
}

B1String B1SECS2MessageS3F17Writable::fullName()
{
    return "Carrier Action Request (S3F17)";
}

bool B1SECS2MessageS3F17Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Carrier Action Request (S3F17)
    <L 5
        <DATAID>
        <CARRIERACTION>
        <CARRIERID>
        <PTN>
        <L n
            <L 2
                <CATTRID>
                <CATTRDATA>
            >
        >
    >
    */
    addList(5, data);
    addData(_dataID, data);
    addData(_carrierAction, data);
    addData(_carrierID, data);
    addData(_ptn, data);
    addList(_cattrData.size(), data);
    for (const auto& cattrDataPair : _cattrData) {
        addList(2, data);
        addData(cattrDataPair.first, data);
        addData(cattrDataPair.second, data);
    }
    return true;
}
