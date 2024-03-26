//
// B1SECS2MessageS6F20Writable.h
//
// Library: B1SECS2
// Package: S6
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_MESSAGE_S6F20_WRITABLE_H
#define _B1SECS2_MESSAGE_S6F20_WRITABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2MessageWritable.h>

namespace BnD {
    class B1SECS2MessageS6F20Writable : public B1SECS2MessageWritable {
    public:
        B1SECS2MessageS6F20Writable(const std::list<B1SECS2DataV>& vs);
        virtual ~B1SECS2MessageS6F20Writable();
    protected:
        const std::list<B1SECS2DataV>& _vs;
    protected:
        virtual B1String fullName() override;
        virtual bool implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data) override;
    };
}   //  !BnD

#endif  // !_B1SECS2_MESSAGE_S6F20_WRITABLE_H
