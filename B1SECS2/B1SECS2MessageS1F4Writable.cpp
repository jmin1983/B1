//
// B1SECS2MessageS1F4Writable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F4Writable.h"

using namespace BnD;

B1SECS2MessageS1F4Writable::B1SECS2MessageS1F4Writable(const std::list<B1SECS2DataSV>& svs)
    : B1SECS2MessageWritable(false, 1, 4, "sendMessageS1F4")
    , _svs(svs)
{
}

B1SECS2MessageS1F4Writable::~B1SECS2MessageS1F4Writable()
{
}

B1String B1SECS2MessageS1F4Writable::fullName()
{
    return "Selected Equipment Status Data (SSD)";
}

bool B1SECS2MessageS1F4Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Selected Equipment Status Data (SSD)
    <L a
        <SV>
    >
    */
    addList(_svs.size(), data);
    for (const auto& sv : _svs) {
        addData(sv, data);
        if (sv->isFormatList()) {
            addData(sv._list, data);
        }
    }
    return true;
}
