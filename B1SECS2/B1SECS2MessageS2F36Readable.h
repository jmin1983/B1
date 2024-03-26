//
// B1SECS2MessageS2F36Readable.h
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_MESSAGE_S2F36_READABLE_H
#define _B1SECS2_MESSAGE_S2F36_READABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2MessageReadable.h>

namespace BnD {
    class B1SECS2MessageS2F36Readable : public B1SECS2MessageReadable {
    public:
        B1SECS2MessageS2F36Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager);
        virtual ~B1SECS2MessageS2F36Readable();
    protected:
        B1SECS2DataLRACK _lrAck;
    protected:
        virtual B1String fullName() override;
        virtual void implToString(B1String* string) override;
        virtual bool implReadData() override;
    public:
        const B1SECS2DataLRACK& lrAck() const { return _lrAck; }
    };
}   //  !BnD

#endif  // !_B1SECS2_MESSAGE_S2F36_READABLE_H
