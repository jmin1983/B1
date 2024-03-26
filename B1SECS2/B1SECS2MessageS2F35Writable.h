//
// B1SECS2MessageS2F35Writable.h
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_MESSAGE_S2F35_WRITABLE_H
#define _B1SECS2_MESSAGE_S2F35_WRITABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2Data.h>
#include <B1SECS2/B1SECS2MessageWritable.h>

namespace BnD {
    class B1SECS2MessageS2F35Writable : public B1SECS2MessageWritable {
    public:
        B1SECS2MessageS2F35Writable(const B1SECS2DataDATAID& dataID, const std::map<B1SECS2DataCEID, std::list<B1SECS2DataRPTID> >& linkEventData);
        virtual ~B1SECS2MessageS2F35Writable();
    protected:
        const B1SECS2DataDATAID& _dataID;
        const std::map<B1SECS2DataCEID, std::list<B1SECS2DataRPTID> >& _linkEventData;
    protected:
        virtual B1String fullName() override;
        virtual bool implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data) override;
    };
}   //  !BnD

#endif  // !_B1SECS2_MESSAGE_S2F35_WRITABLE_H
