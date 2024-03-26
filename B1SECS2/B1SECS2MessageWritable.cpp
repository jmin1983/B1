//
// B1SECS2MessageWritable.cpp
//
// Library: B1SECS2
// Package: Messages
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageWritable.h"
#include "B1SECS2Consts.h"
#include "B1SECS2Data.h"

using namespace BnD;

B1SECS2MessageWritable::B1SECS2MessageWritable(bool wait, uint8 stream, uint8 function, B1String&& messageName)
    : B1SECS2Message(wait, stream, function, std::move(messageName))
    , _items()
{
}

B1SECS2MessageWritable::~B1SECS2MessageWritable()
{
}

void B1SECS2MessageWritable::appendToLogStringDecreaseDepthRecursive(int32* depth, std::list<int32>* remainDepths, B1String* log)
{
    if (remainDepths->empty() != true) {
        *remainDepths->rbegin() = remainDepths->back() - 1;
        if (remainDepths->back() < 1) {
            (*depth)--;
            remainDepths->pop_back();
            appendStringDepth(*depth, ">", log);
            appendToLogStringDecreaseDepthRecursive(depth, remainDepths, log);
        }
    }
}

void B1SECS2MessageWritable::appendToLogString(const std::list<SPB1SECS2Data>& data, B1String* log)
{
    int32 depth = 0;
    std::list<int32> remainDepths;
    for (auto d : data) {
        if (d == NULL) {
            assert(false);
            appendStringDepth(depth, "NULL", log);
            continue;
        }
        if (d->isFormatList()) {
            uint32 count = static_cast<const B1SECS2DataLIST*>(d.get())->count();
            appendStringDepthList(depth, count, log);
            depth++;
            remainDepths.push_back(count);
            if (count < 1)
                appendToLogStringDecreaseDepthRecursive(&depth, &remainDepths, log);
        }
        else {
            appendStringDepth(depth, d->toString(), log);
            appendToLogStringDecreaseDepthRecursive(&depth, &remainDepths, log);
        }
    }
    assert(remainDepths.empty());
    assert(depth == 0);
}

void B1SECS2MessageWritable::addList(size_t count, std::list<SPB1SECS2Data>* result)
{
    result->push_back(std::make_shared<B1SECS2DataLIST>(static_cast<uint32>(count)));
}

void B1SECS2MessageWritable::addData(const SPB1SECS2Data& data, std::list<SPB1SECS2Data>* result)
{
    result->push_back(data);
}

void B1SECS2MessageWritable::addData(const B1SECS2DataBase& data, std::list<SPB1SECS2Data>* result)
{
    result->push_back(data.data());
}

void B1SECS2MessageWritable::addData(const std::list<SPB1SECS2Data>& data, std::list<SPB1SECS2Data>* result)
{
    result->insert(result->end(), data.begin(), data.end());
}

bool B1SECS2MessageWritable::writeData()
{
    B1String log = toString();
    std::list<std::shared_ptr<B1SECS2Data> > data;
    if (implWriteData(&data) != true) {
        appendStringDepth(0, "ERROR", &log);
        return false;
    }
    appendToLogString(data, &log);
    B1LOG(log.cString());
    std::vector<std::vector<uint8> >(data.size()).swap(_items);
    size_t index = 0;
    for (auto d : data) {
        if (d)
            d->writeTo(&_items[index++]);
    }
    return true;
}
