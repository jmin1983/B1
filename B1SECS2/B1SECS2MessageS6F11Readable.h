//
// B1SECS2MessageS6F11Readable.h
//
// Library: B1SECS2
// Package: S6
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_MESSAGE_S6F11_READABLE_H
#define _B1SECS2_MESSAGE_S6F11_READABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2MessageReadable.h>

namespace BnD {
    class B1SECS2MessageS6F11Readable : public B1SECS2MessageReadable {
    public:
        B1SECS2MessageS6F11Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager);
        virtual ~B1SECS2MessageS6F11Readable();
    protected:
        B1SECS2DataDATAID _dataID;
        B1SECS2DataCEID _ceID;
        std::map<B1SECS2DataRPTID, std::vector<B1SECS2DataV> > _reportsData;
    protected:
        virtual B1String fullName() override;
        virtual void implToString(B1String* string) override;
        virtual bool implReadData() override;
    public:
        const B1SECS2DataDATAID& dataID() const { return _dataID; }
        const B1SECS2DataCEID& ceID() const { return _ceID; }
        const std::map<B1SECS2DataRPTID, std::vector<B1SECS2DataV> >& reportsData() const { return _reportsData; }
    };
}   //  !BnD

#endif  // !_B1SECS2_MESSAGE_S6F11_READABLE_H
