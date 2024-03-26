//
// B1SECS2MessageS2F50Writable.h
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_MESSAGE_S2F50_WRITABLE_H
#define _B1SECS2_MESSAGE_S2F50_WRITABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2MessageWritable.h>

namespace BnD {
    class B1SECS2MessageS2F50Writable : public B1SECS2MessageWritable {
    public:
        B1SECS2MessageS2F50Writable(const B1SECS2DataHCACK& hcAck, const std::map<B1SECS2DataCPNAME, B1SECS2DataCEPACK>& cps);
        virtual ~B1SECS2MessageS2F50Writable();
    protected:
        const B1SECS2DataHCACK& _hcAck;
        const std::map<B1SECS2DataCPNAME, B1SECS2DataCEPACK>& _cps;
    protected:
        virtual B1String fullName() override;
        virtual bool implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data) override;
    };
}   //  !BnD

#endif  // !_B1SECS2_MESSAGE_S2F50_WRITABLE_H
