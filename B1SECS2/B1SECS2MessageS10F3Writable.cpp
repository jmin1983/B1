//
// B1SECS2MessageS10F3Writable.cpp
//
// Library: B1SECS2
// Package: S10
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS10F3Writable.h"

using namespace BnD;

B1SECS2MessageS10F3Writable::B1SECS2MessageS10F3Writable(const B1SECS2DataTID& tid, const B1SECS2DataTEXT& text)
    : B1SECS2MessageWritable(true, 10, 3, "sendMessageS10F3")
    , _tid(tid)
    , _text(text)
{
}

B1SECS2MessageS10F3Writable::~B1SECS2MessageS10F3Writable()
{
}

B1String B1SECS2MessageS10F3Writable::fullName()
{
    return "Terminal Display, Single (VTN)";
}

bool B1SECS2MessageS10F3Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Terminal Display, Single (VTN)
    <L 2
        <TID>
        <TEXT>
    >
    */
    addList(2, data);
    addData(_tid, data);
    addData(_text, data);
    return true;
}
