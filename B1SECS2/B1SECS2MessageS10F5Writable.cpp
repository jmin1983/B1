//
// B1SECS2MessageS10F5Writable.cpp
//
// Library: B1SECS2
// Package: S10
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS10F5Writable.h"

using namespace BnD;

B1SECS2MessageS10F5Writable::B1SECS2MessageS10F5Writable(const B1SECS2DataTID& tid, const std::vector<B1SECS2DataTEXT>& texts)
    : B1SECS2MessageWritable(true, 10, 5, "sendMessageS10F5")
    , _tid(tid)
    , _texts(texts)
{
}

B1SECS2MessageS10F5Writable::~B1SECS2MessageS10F5Writable()
{
}

B1String B1SECS2MessageS10F5Writable::fullName()
{
    return "Terminal Display, Multi-Block (VTN)";
}

bool B1SECS2MessageS10F5Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Terminal Display, Multi-Block (VTN)
    <L 2
        <TID>
        <L n
            <TEXT>
        >
    >
    */
    addList(2, data);
    addData(_tid, data);
    addList(_texts.size(), data);
    for (const auto& text : _texts) {
        addData(text, data);
    }
    return true;
}
