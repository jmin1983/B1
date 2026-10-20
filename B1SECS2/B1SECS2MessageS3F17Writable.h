//
// B1SECS2MessageS3F17Writable.h
//
// Library: B1SECS2
// Package: S3
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_MESSAGE_S3F17_WRITABLE_H
#define _B1SECS2_MESSAGE_S3F17_WRITABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2MessageWritable.h>

namespace BnD {
    class B1SECS2MessageS3F17Writable : public B1SECS2MessageWritable {
    public:
        B1SECS2MessageS3F17Writable(const B1SECS2DataDATAID& dataID,
                                    const B1SECS2DataCARRIERACTION& carrierAction,
                                    const B1SECS2DataCARRIERID& carrierID,
                                    const B1SECS2DataPTN& ptn,
                                    const std::map<B1SECS2DataCATTRID, B1SECS2DataCATTRDATA>& cattrData);
        virtual ~B1SECS2MessageS3F17Writable();
    protected:
        const B1SECS2DataDATAID& _dataID;
        const B1SECS2DataCARRIERACTION& _carrierAction;
        const B1SECS2DataCARRIERID& _carrierID;
        const B1SECS2DataPTN& _ptn;
        const std::map<B1SECS2DataCATTRID, B1SECS2DataCATTRDATA>& _cattrData;
    protected:
        virtual B1String fullName() override;
        virtual bool implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data) override;
    };
}   //  !BnD

#endif  // !_B1SECS2_MESSAGE_S3F17_WRITABLE_H
