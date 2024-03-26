//
// B1SECS2MessageS64F1Writable.h
//
// Library: B1SECS2
// Package: Custom
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_MESSAGE_S64F1_WRITABLE_H
#define _B1SECS2_MESSAGE_S64F1_WRITABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2MessageWritable.h>

namespace BnD {
    class B1SECS2MessageS64F1Writable : public B1SECS2MessageWritable {
    public:
        B1SECS2MessageS64F1Writable(const B1SECS2DataRCMD& rcmd, const std::map<B1SECS2DataCPNAME, B1SECS2DataCPVAL>& cps);
        virtual ~B1SECS2MessageS64F1Writable();
    protected:
        const B1SECS2DataRCMD& _rcmd;
        const std::map<B1SECS2DataCPNAME, B1SECS2DataCPVAL>& _cps;
    protected:
        virtual B1String fullName() override;
        virtual bool implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data) override;
    };
}   //  !BnD

#endif  // !_B1SECS2_MESSAGE_S64F1_WRITABLE_H
