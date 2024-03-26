//
// B1SECS2MessageS9.cpp
//
// Library: B1SECS2
// Package: S9
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS9.h"

using namespace BnD;

B1SECS2MessageS9::B1SECS2MessageS9(uint8 function, B1String&& messageName, const std::vector<uint8>& messageHeader)
    : B1SECS2MessageWritable(false, 9, function, std::move(messageName))
    , _messageHeaderBin(std::make_shared<B1SECS2DataBIN>(messageHeader))
{
    assert(messageHeader.size() == 10);
}

bool B1SECS2MessageS9::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /*
    <MHEAD> DATA_FORMAT_BIN, 10
    */
    data->push_back(_messageHeaderBin);
    return true;
}

