//
// B1SECS2MessageS1F12Readable.h
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_MESSAGE_S1F12_READABLE_H
#define _B1SECS2_MESSAGE_S1F12_READABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2MessageReadable.h>

namespace BnD {
    class B1SECS2MessageS1F12Readable : public B1SECS2MessageReadable {
    public:
        B1SECS2MessageS1F12Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager);
        virtual ~B1SECS2MessageS1F12Readable();
    protected:
        struct InternalData {
            InternalData(const B1SECS2DataManager* dataManager) : _svID(dataManager), _svName(dataManager), _units(dataManager) {}
            B1SECS2DataSVID _svID;
            B1SECS2DataSVNAME _svName;
            B1SECS2DataUNITS _units;
        };
        std::vector<InternalData> _internalData;
    protected:
        virtual B1String fullName() override;
        virtual void implToString(B1String* string) override;
        virtual bool implReadData() override;
    public:
        size_t internalDataSize() const { return _internalData.size(); }
        const B1SECS2DataSVID& svID(size_t index) const { return _internalData[index]._svID; }
        const B1SECS2DataSVNAME& svName(size_t index) const { return _internalData[index]._svName; }
        const B1SECS2DataUNITS& units(size_t index) const { return _internalData[index]._units; }
    };
}   //  !BnD

#endif  // !_B1SECS2_MESSAGE_S1F12_READABLE_H
