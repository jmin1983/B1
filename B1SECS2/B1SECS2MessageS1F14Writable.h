//
// B1SECS2MessageS1F14Writable.h
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_MESSAGE_S1F14_WRITABLE_H
#define _B1SECS2_MESSAGE_S1F14_WRITABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2MessageWritable.h>

namespace BnD {
    class B1SECS2MessageS1F14Writable : public B1SECS2MessageWritable {
    public:
        B1SECS2MessageS1F14Writable(const B1SECS2DataCOMMACK& commAck, const B1SECS2DataMDLN* mdln = NULL, const B1SECS2DataSOFTREV* softRev = NULL);
        virtual ~B1SECS2MessageS1F14Writable();
    protected:
        const B1SECS2DataCOMMACK& _commAck;
        const B1SECS2DataMDLN* _mdln;
        const B1SECS2DataSOFTREV* _softRev;
    protected:
        virtual B1String fullName() override;
        virtual bool implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data) override;
    protected:
        bool isSentFromHost() const { return NULL == _mdln || NULL == _softRev; }
    };
}   //  !BnD

#endif  // !_B1SECS2_MESSAGE_S1F14_WRITABLE_H
