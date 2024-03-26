//
// B1SECS2MessageS5F8Readable.h
//
// Library: B1SECS2
// Package: S5
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_MESSAGE_S5F8_READABLE_H
#define _B1SECS2_MESSAGE_S5F8_READABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2MessageReadable.h>

namespace BnD {
    class B1SECS2MessageS5F8Readable : public B1SECS2MessageReadable {
    public:
        B1SECS2MessageS5F8Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager);
        virtual ~B1SECS2MessageS5F8Readable();
    protected:
        struct InternalData {
            InternalData(const B1SECS2DataManager* dataManager) : _alCD(dataManager), _alID(dataManager), _alTX(dataManager) {}
            B1SECS2DataALCD _alCD;
            B1SECS2DataALID _alID;
            B1SECS2DataALTX _alTX;
        };
        std::vector<InternalData> _internalData;
    protected:
        virtual B1String fullName() override;
        virtual void implToString(B1String* string) override;
        virtual bool implReadData() override;
    public:
        size_t internalDataSize() const { return _internalData.size(); }
        const B1SECS2DataALCD& alCD(size_t index) const { return _internalData[index]._alCD; }
        const B1SECS2DataALID& alID(size_t index) const { return _internalData[index]._alID; }
        const B1SECS2DataALTX& alTX(size_t index) const { return _internalData[index]._alTX; }
    };
}   //  !BnD

#endif  // !_B1SECS2_MESSAGE_S5F8_READABLE_H
