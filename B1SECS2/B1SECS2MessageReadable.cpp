//
// B1SECS2MessageReadable.cpp
//
// Library: B1SECS2
// Package: Messages
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageReadable.h"

using namespace BnD;

B1SECS2MessageReadable::B1SECS2MessageReadable(bool wait, uint8 stream, uint8 function, B1String&& messageName, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2Message(wait, stream, function, std::move(messageName))
    , _data(data)
    , _dataLength(dataLength)
    , _dataPos(0)
    , _dataManager(dataManager)
{
}

B1SECS2MessageReadable::~B1SECS2MessageReadable()
{
}

bool B1SECS2MessageReadable::readDummyList(uint32 expectedSize)
{
    B1SECS2DataLIST dummy;
    if (readSECS2Data(&dummy) != true) {
        B1LOG("%s read dummy_list failed", _messageName.cString());
        return false;
    }
    if (dummy.count() != expectedSize) {
        B1LOG("%s invalid dummy_list count:expected[%u], real[%u]", _messageName.cString(), expectedSize, dummy.count());
        return false;
    }
    return true;
}

bool B1SECS2MessageReadable::readSECS2Data(B1SECS2Data* data)
{
    if (data->readFrom(_data, _dataLength, &_dataPos) != true) {
        B1LOG("%s data_format[%o] read failed", _messageName.cString(), data->format());
        return false;
    }
    return true;
}

bool B1SECS2MessageReadable::readSECS2Data(B1SECS2DataBase* data)
{
    return readSECS2Data(data->get());
}

bool B1SECS2MessageReadable::readSECS2DataList(std::shared_ptr<B1SECS2DataLIST> list, std::list<SPB1SECS2Data>* result)
{
    for (uint32 i = 0; i < list->count(); ++i) {
        auto data = readUnknownSECS2Data();
        if (data == NULL) {
            return false;
        }
        result->push_back(data);
        if (data->isFormatList()) {
            if (readSECS2DataList(std::dynamic_pointer_cast<B1SECS2DataLIST>(data), result) != true) {
                return false;
            }
        }
    }
    return true;
}

std::shared_ptr<B1SECS2Data> B1SECS2MessageReadable::readUnknownSECS2Data()
{
    std::shared_ptr<B1SECS2Data> data(B1SECS2Data::createFromUnknownFormat(_data, _dataLength, &_dataPos));
    if (data == NULL) {
        B1LOG("%s read_unknown_data failed", _messageName.cString());
    }
    return data;
}

bool B1SECS2MessageReadable::readData()
{
    bool result = implReadData();
    B1String log;
    if (result != true)
        appendStringDepth0("ERROR", &log);
    log += toString();
    B1LOG(log.cString());
    return result;
}
