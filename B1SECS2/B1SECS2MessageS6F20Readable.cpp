//
// B1SECS2MessageS6F20Readable.cpp
//
// Library: B1SECS2
// Package: S6
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS6F20Readable.h"

using namespace BnD;

B1SECS2MessageS6F20Readable::B1SECS2MessageS6F20Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 6, 20, "recvMessageS6F20", data, dataLength, dataManager)
    , _vs()
{
}

B1SECS2MessageS6F20Readable::~B1SECS2MessageS6F20Readable()
{
}

B1String B1SECS2MessageS6F20Readable::fullName()
{
    return "Individual Report Data (IRD)";
}

void B1SECS2MessageS6F20Readable::implToString(B1String* string)
{
    appendStringDepth0List(_vs.size(), string);
    for (std::vector<B1SECS2DataV>::const_iterator itr(_vs.begin()); itr != _vs.end(); ++itr) {
        if ((*itr)->isFormatList()) {
            appendListToString(&itr, 1, string);
        }
        else {
            appendStringDepth1((*itr)->toString().cString(), string);
        }
    }
    appendStringDepth0Closer(string);
}

bool B1SECS2MessageS6F20Readable::implReadData()
{
    /* Individual Report Data (IRD)
    W
    <L a
        <V>
    >
    */
    B1SECS2DataLIST list;
    if (readSECS2Data(&list) != true) {
        return false;
    }
    _vs.clear();
    for (uint32 i = 0; i < list.count(); ++i) {
        auto data = readUnknownSECS2Data();
        if (data == NULL) {
            return false;
        }
        if (data->isFormatList()) {
            B1SECS2DataV v(data);
            readSECS2DataList(std::dynamic_pointer_cast<B1SECS2DataLIST>(data), &v._list);
            _vs.push_back(v);
        }
        else {
            _vs.push_back(data);
        }
    }
    return true;
}

void B1SECS2MessageS6F20Readable::appendListToString(std::vector<B1SECS2DataV>::const_iterator* itrList, int32 depth, B1String* string) const
{
    auto list = dynamic_cast<const B1SECS2DataLIST*>((*itrList)->get());
    if (list == NULL) {
        assert(false);
        B1LOG("invalid list type detected");
        return;
    }
    appendStringDepthList(depth++, list->count(), string);
    for (; (*itrList) != _vs.end(); ++(*itrList)) {
        if ((*(*itrList))->isFormatList()) {
            appendListToString(itrList, depth, string);
        }
        else {
            appendStringDepth(depth, (*itrList)->get()->toString().cString(), string);
        }
    }
    appendStringDepthCloser(--depth, string);
}
