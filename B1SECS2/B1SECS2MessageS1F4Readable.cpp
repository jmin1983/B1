//
// B1SECS2MessageS1F4Readable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F4Readable.h"
#include "B1SECS2Data.h"

using namespace BnD;

B1SECS2MessageS1F4Readable::B1SECS2MessageS1F4Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 1, 4, "recvMessageS1F4", data, dataLength, dataManager)
    , _svs()
{
}

B1SECS2MessageS1F4Readable::~B1SECS2MessageS1F4Readable()
{
}

B1String B1SECS2MessageS1F4Readable::fullName()
{
    return "Selected Equipment Status Data (SSD)";
}

void B1SECS2MessageS1F4Readable::implToString(B1String* string)
{
    appendStringDepth0List(_svs.size(), string);
    for (std::vector<B1SECS2DataSV>::const_iterator itr(_svs.begin()); itr != _svs.end(); ++itr) {
        appendStringDepth1((*itr)->toString().cString(), string);
        if ((*itr)->isFormatList()) {
            std::list<SPB1SECS2Data>::const_iterator index;
            index = itr->_list.begin();
            appendListToString(itr->_list, 2, static_cast<int32>(itr->_list.size()), &index, string);
        }
    }
    appendStringDepth0Closer(string);
}

bool B1SECS2MessageS1F4Readable::implReadData()
{
    /* Selected Equipment Status Request (SSR)
    W
    <L a
        <SV>
    >
    */
    B1SECS2DataLIST list;
    if (readSECS2Data(&list) != true) {
        return false;
    }
    _svs.clear();
    for (uint32 i = 0; i < list.count(); ++i) {
        auto data = readUnknownSECS2Data();
        if (data == NULL) {
            return false;
        }
        if (data->isFormatList()) {
            B1SECS2DataSV sv(data);
            readSECS2DataList(std::dynamic_pointer_cast<B1SECS2DataLIST>(data), &sv._list);
            _svs.push_back(sv);
        }
        else {
            _svs.push_back(data);
        }
    }
    return true;
}

void B1SECS2MessageS1F4Readable::appendListToString(const std::list<SPB1SECS2Data>& subData, int32 depth, int32 count, std::list<SPB1SECS2Data>::const_iterator* itrList, B1String* string) const
{
    while ((*itrList) != subData.end() && count > 0) {
        SPB1SECS2Data currentData = *(*itrList);
        appendStringDepth(depth, currentData->toString(), string);
        ++(*itrList);
        count--;
        if (currentData->isFormatList()) {
            auto list = dynamic_cast<const B1SECS2DataLIST*>(currentData.get());
            if (list == NULL) {
                assert(false);
                B1LOG("invalid list type detected");
                return;
            }
            appendListToString(subData, depth + 1, list->count(), itrList, string);
        }
    }
    appendStringDepthCloser(depth - 1, string);
}
