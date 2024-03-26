//
// B1SECS2MessageS1F22Readable.h
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_MESSAGE_S1F22_READABLE_H
#define _B1SECS2_MESSAGE_S1F22_READABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2MessageReadable.h>

namespace BnD {
    class B1SECS2MessageS1F22Readable : public B1SECS2MessageReadable {
    public:
        B1SECS2MessageS1F22Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager);
        virtual ~B1SECS2MessageS1F22Readable();
    protected:
        struct InternalData {
            InternalData(const B1SECS2DataManager* dataManager) : _vid(dataManager), _dvvalName(dataManager), _units(dataManager) {}
            B1SECS2DataVID _vid;
            B1SECS2DataDVVALNAME _dvvalName;
            B1SECS2DataUNITS _units;
        };
        std::vector<InternalData> _internalData;
    protected:
        virtual B1String fullName() override;
        virtual void implToString(B1String* string) override;
        virtual bool implReadData() override;
    public:
        size_t internalDataSize() const { return _internalData.size(); }
        const B1SECS2DataVID& svID(size_t index) const { return _internalData[index]._vid; }
        const B1SECS2DataDVVALNAME& svName(size_t index) const { return _internalData[index]._dvvalName; }
        const B1SECS2DataUNITS& uints(size_t index) const { return _internalData[index]._units; }
    };
}   //  !BnD

#endif  // !_B1SECS2_MESSAGE_S1F22_READABLE_H
