//
// B1SECS2MessageS10F5Writable.h
//
// Library: B1SECS2
// Package: S10
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_MESSAGE_S10F5_WRITABLE_H
#define _B1SECS2_MESSAGE_S10F5_WRITABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2MessageWritable.h>

namespace BnD {
    class B1SECS2MessageS10F5Writable : public B1SECS2MessageWritable {
    public:
        B1SECS2MessageS10F5Writable(const B1SECS2DataTID& tid, const std::vector<B1SECS2DataTEXT>& texts);
        virtual ~B1SECS2MessageS10F5Writable();
    protected:
        const B1SECS2DataTID& _tid;
        const std::vector<B1SECS2DataTEXT>& _texts;
    protected:
        virtual B1String fullName() override;
        virtual bool implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data) override;
    };
}   //  !BnD

#endif  // !_B1SECS2_MESSAGE_S10F5_WRITABLE_H
