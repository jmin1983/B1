//
// B1SECS2MessageS1F2ReadableReadable.h
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_MESSAGE_S1F2_READABLE_H
#define _B1SECS2_MESSAGE_S1F2_READABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2MessageReadable.h>

namespace BnD {
    class B1SECS2MessageS1F2Readable : public B1SECS2MessageReadable {
    public:
        B1SECS2MessageS1F2Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager);
        virtual ~B1SECS2MessageS1F2Readable();
    protected:
        B1SECS2DataLIST _list;    //  count 0 if [H->E] else [E->H]
        B1SECS2DataMDLN _mdln;
        B1SECS2DataSOFTREV _softRev;
    protected:
        virtual B1String fullName() override;
        virtual void implToString(B1String* string) override;
        virtual bool implReadData() override;
    protected:
        bool isSentFromHost() const { return _list.count() == 0; }
    public:
        const B1SECS2DataMDLN& mdln() const { return _mdln; }
        const B1SECS2DataSOFTREV& softRev() const { return _softRev; }
    };
}   //  !BnD

#endif  // !_B1SECS2_MESSAGE_S1F2_READABLE_H
