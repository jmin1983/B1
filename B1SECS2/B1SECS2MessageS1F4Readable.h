//
// B1SECS2MessageS1F4Readable.h
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_MESSAGE_S1F4_READABLE_H
#define _B1SECS2_MESSAGE_S1F4_READABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2MessageReadable.h>

namespace BnD {
    class B1SECS2MessageS1F4Readable : public B1SECS2MessageReadable {
    public:
        B1SECS2MessageS1F4Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager);
        virtual ~B1SECS2MessageS1F4Readable();
    protected:
        std::vector<B1SECS2DataSV> _svs;
    protected:
        virtual B1String fullName() override;
        virtual void implToString(B1String* string) override;
        virtual bool implReadData() override;
    protected:
        void appendListToString(const std::list<SPB1SECS2Data>& subData, int32 depth, int32 count, std::list<SPB1SECS2Data>::const_iterator* itrList, B1String* string) const;
    public:
        const std::vector<B1SECS2DataSV>& svs() const { return _svs; }
    };
}   //  !BnD

#endif  // !_B1SECS2_MESSAGE_S1F4_READABLE_H
