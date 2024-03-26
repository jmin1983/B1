//
// B1SECS2MessageS5F5Readable.h
//
// Library: B1SECS2
// Package: S5
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_MESSAGE_S5F5_READABLE_H
#define _B1SECS2_MESSAGE_S5F5_READABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2MessageReadable.h>

namespace BnD {
    class B1SECS2MessageS5F5Readable : public B1SECS2MessageReadable {
    public:
        B1SECS2MessageS5F5Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager);
        virtual ~B1SECS2MessageS5F5Readable();
    protected:
        B1SECS2DataALID _alID;
    protected:
        virtual B1String fullName() override;
        virtual void implToString(B1String* string) override;
        virtual bool implReadData() override;
    public:
        const B1SECS2DataALID& alID() const { return _alID; }
    };
}   //  !BnD

#endif  // !_B1SECS2_MESSAGE_S5F5_READABLE_H
