//
// B1SECS2MessageS1F3Readable.h
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_MESSAGE_S1F3_READABLE_H
#define _B1SECS2_MESSAGE_S1F3_READABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2MessageReadable.h>

namespace BnD {
    class B1SECS2MessageS1F3Readable : public B1SECS2MessageReadable {
    public:
        B1SECS2MessageS1F3Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager);
        virtual ~B1SECS2MessageS1F3Readable();
    protected:
        std::vector<B1SECS2DataSVID> _svIDs;
    protected:
        virtual B1String fullName() override;
        virtual void implToString(B1String* string) override;
        virtual bool implReadData() override;
    public:
        const std::vector<B1SECS2DataSVID>& svIDs() const { return _svIDs; }
    };
}   //  !BnD

#endif  // !_B1SECS2_MESSAGE_S1F3_READABLE_H
