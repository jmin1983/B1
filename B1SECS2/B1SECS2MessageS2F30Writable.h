//
// B1SECS2MessageS2F30Writable.h
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_MESSAGE_S2F30_WRITABLE_H
#define _B1SECS2_MESSAGE_S2F30_WRITABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2MessageWritable.h>

namespace BnD {
    class B1SECS2MessageS2F30Writable : public B1SECS2MessageWritable {
    public:
        B1SECS2MessageS2F30Writable(const std::vector<B1SECS2DataECID>& ecIDs,
                                    const std::vector<B1SECS2DataECNAME>& ecNames,
                                    const std::vector<B1SECS2DataECMIN>& ecMins,
                                    const std::vector<B1SECS2DataECMAX>& ecMaxes,
                                    const std::vector<B1SECS2DataECDEF>& ecDefs,
                                    const std::vector<B1SECS2DataUNITS>& units);
        virtual ~B1SECS2MessageS2F30Writable();
    protected:
        const std::vector<B1SECS2DataECID>& _ecIDs;
        const std::vector<B1SECS2DataECNAME>& _ecNames;
        const std::vector<B1SECS2DataECMIN>& _ecMins;
        const std::vector<B1SECS2DataECMAX>& _ecMaxes;
        const std::vector<B1SECS2DataECDEF>& _ecDefs;
        const std::vector<B1SECS2DataUNITS>& _units;
    protected:
        virtual B1String fullName() override;
        virtual bool implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data) override;
    };
}   //  !BnD

#endif  // !_B1SECS2_MESSAGE_S2F30_WRITABLE_H
