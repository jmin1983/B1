//
// B1SECS2MessageS2F49Writable.h
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_MESSAGE_S2F49_WRITABLE_H
#define _B1SECS2_MESSAGE_S2F49_WRITABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2MessageWritable.h>

namespace BnD {
    class B1SECS2MessageS2F49Writable : public B1SECS2MessageWritable {
    public:
        B1SECS2MessageS2F49Writable(const B1SECS2DataDATAID& dataID, const B1SECS2DataOBJSPEC& objSpec, const B1SECS2DataRCMD& rcmd,
                                  const std::map<B1SECS2DataCPNAME, B1SECS2DataCEPVAL>& ceps);
        virtual ~B1SECS2MessageS2F49Writable();
    protected:
        B1SECS2DataDATAID _dataID;
        B1SECS2DataOBJSPEC _objSpec;
        B1SECS2DataRCMD _rcmd;
        const std::map<B1SECS2DataCPNAME, B1SECS2DataCEPVAL>& _ceps;
    protected:
        virtual B1String fullName() override;
        virtual bool implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data) override;
    };
}   //  !BnD

#endif  // !_B1SECS2_MESSAGE_S2F49_WRITABLE_H
