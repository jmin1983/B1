//
// B1SECS2ClientSessionImplS10Send.cpp
//
// Library: B1SECS2
// Package: Client
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2ClientSession.h"
#include "B1SECS2MessageS10F3Writable.h"
#include "B1SECS2MessageS10F5Writable.h"

using namespace BnD;

bool B1SECS2ClientSession::sendMessageS10F3(const B1SECS2DataTID& tid, const B1SECS2DataTEXT& text)
{
    B1SECS2MessageS10F3Writable message(tid, text);
    if (message.writeData() != true) {
        return false;
    }
    sendDataMessage(makeNewSystemBytes(), message);
    return true;
}

bool B1SECS2ClientSession::sendMessageS10F5(const B1SECS2DataTID& tid, const std::vector<B1SECS2DataTEXT>& texts)
{
    B1SECS2MessageS10F5Writable message(tid, texts);
    if (message.writeData() != true) {
        return false;
    }
    sendDataMessage(makeNewSystemBytes(), message);
    return true;
}
