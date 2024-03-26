//
// B1SECS2MessageS1F24Readable.h
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_MESSAGE_S1F24_READABLE_H
#define _B1SECS2_MESSAGE_S1F24_READABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2MessageReadable.h>

namespace BnD {
    class B1SECS2MessageS1F24Readable : public B1SECS2MessageReadable {
    public:
        B1SECS2MessageS1F24Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager);
        virtual ~B1SECS2MessageS1F24Readable();
    protected:
        struct InternalData {
            InternalData(const B1SECS2DataManager* dataManager) : _ceID(dataManager), _ceName(dataManager), _vIDs() {}
            B1SECS2DataCEID _ceID;
            B1SECS2DataCENAME _ceName;
            std::vector<B1SECS2DataVID> _vIDs;
        };
        std::vector<InternalData> _internalData;
    protected:
        virtual B1String fullName() override;
        virtual void implToString(B1String* string) override;
        virtual bool implReadData() override;
    public:
        size_t internalDataSize() const { return _internalData.size(); }
        const B1SECS2DataCEID& svID(size_t index) const { return _internalData[index]._ceID; }
        const B1SECS2DataCENAME& svName(size_t index) const { return _internalData[index]._ceName; }
        const std::vector<B1SECS2DataVID>& vIDs(size_t index) const { return _internalData[index]._vIDs; }
    };
}   //  !BnD

#endif  // !_B1SECS2_MESSAGE_S1F24_READABLE_H
