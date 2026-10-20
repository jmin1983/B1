//
// B1SECS2MessageS3F17Readable.h
//
// Library: B1SECS2
// Package: S3
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_MESSAGE_S3F17_READABLE_H
#define _B1SECS2_MESSAGE_S3F17_READABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2MessageReadable.h>

namespace BnD {
    class B1SECS2MessageS3F17Readable : public B1SECS2MessageReadable {
    public:
        B1SECS2MessageS3F17Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager);
        virtual ~B1SECS2MessageS3F17Readable();
    protected:
        B1SECS2DataDATAID _dataID;
        B1SECS2DataCARRIERACTION _carrierAction;
        B1SECS2DataCARRIERID _carrierID;
        B1SECS2DataPTN _ptn;
        std::map<B1SECS2DataCATTRID, B1SECS2DataCATTRDATA> _cattrData;
    protected:
        virtual B1String fullName() override;
        virtual void implToString(B1String* string) override;
        virtual bool implReadData() override;
    public:
        const B1SECS2DataDATAID& dataID() const { return _dataID; }
        const B1SECS2DataCARRIERACTION& carrierAction() const { return _carrierAction; }
        const B1SECS2DataCARRIERID& carrierID() const { return _carrierID; }
        const B1SECS2DataPTN& ptn() const { return _ptn; }
        const std::map<B1SECS2DataCATTRID, B1SECS2DataCATTRDATA>& cattrData() const { return _cattrData; }
    };
}   //  !BnD

#endif  // !_B1SECS2_MESSAGE_S3F17_READABLE_H
