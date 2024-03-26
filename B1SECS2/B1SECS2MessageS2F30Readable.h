//
// B1SECS2MessageS2F30Readable.h
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_MESSAGE_S2F30_READABLE_H
#define _B1SECS2_MESSAGE_S2F30_READABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2MessageReadable.h>

namespace BnD {
    class B1SECS2MessageS2F30Readable : public B1SECS2MessageReadable {
    public:
        B1SECS2MessageS2F30Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager);
        virtual ~B1SECS2MessageS2F30Readable();
    protected:
        struct InternalData {
            InternalData(const B1SECS2DataManager* dataManager)
                : _ecID(dataManager), _ecName(dataManager), _ecMin(), _ecMax(), _ecDef(), _units(dataManager) {}
            B1SECS2DataECID _ecID;
            B1SECS2DataECNAME _ecName;
            std::shared_ptr<B1SECS2DataECMIN> _ecMin;
            std::shared_ptr<B1SECS2DataECMAX> _ecMax;
            std::shared_ptr<B1SECS2DataECDEF> _ecDef;
            B1SECS2DataUNITS _units;
        };
        std::vector<InternalData> _internalData;
    protected:
        virtual B1String fullName() override;
        virtual void implToString(B1String* string) override;
        virtual bool implReadData() override;
    public:
        size_t internalDataSize() const { return _internalData.size(); }
        const B1SECS2DataECID& ecID(size_t index) const { return _internalData[index]._ecID; }
        const B1SECS2DataECNAME& ecName(size_t index) const { return _internalData[index]._ecName; }
        std::shared_ptr<B1SECS2DataECMIN> ecMin(size_t index) const { return _internalData[index]._ecMin; }
        std::shared_ptr<B1SECS2DataECMAX> ecMax(size_t index) const { return _internalData[index]._ecMax; }
        std::shared_ptr<B1SECS2DataECDEF> ecDef(size_t index) const { return _internalData[index]._ecDef; }
        const B1SECS2DataUNITS& units(size_t index) const { return _internalData[index]._units; }
    };
}   //  !BnD

#endif  // !_B1SECS2_MESSAGE_S2F30_READABLE_H
