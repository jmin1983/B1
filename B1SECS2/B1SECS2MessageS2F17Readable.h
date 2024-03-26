//
// B1SECS2MessageS2F17Readable.h
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_MESSAGE_S2F17_READABLE_H
#define _B1SECS2_MESSAGE_S2F17_READABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2MessageReadable.h>

namespace BnD {
    class B1SECS2MessageS2F17Readable : public B1SECS2MessageReadable {
    public:
        B1SECS2MessageS2F17Readable(bool wait, const uint8* data, uint32 dataLength);
        virtual ~B1SECS2MessageS2F17Readable();
    protected:
        virtual B1String fullName() override;
        virtual bool implReadData() override;
    };
}   //  !BnD

#endif  // !_B1SECS2_MESSAGE_S2F17_READABLE_H
