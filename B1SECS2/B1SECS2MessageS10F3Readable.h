//
// B1SECS2MessageS10F3Readable.h
//
// Library: B1SECS2
// Package: S10
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_MESSAGE_S10F3_READABLE_H
#define _B1SECS2_MESSAGE_S10F3_READABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2MessageReadable.h>

namespace BnD {
    class B1SECS2MessageS10F3Readable : public B1SECS2MessageReadable {
    public:
        B1SECS2MessageS10F3Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager);
        virtual ~B1SECS2MessageS10F3Readable();
    protected:
        B1SECS2DataTID _tid;
        B1SECS2DataTEXT _text;
    protected:
        virtual B1String fullName() override;
        virtual void implToString(B1String* string) override;
        virtual bool implReadData() override;
    protected:
        B1String replaceSpecialCharacter(const B1String& text) const;
    public:
        const B1SECS2DataTID& tid() const { return _tid; }
        const B1SECS2DataTEXT& text() const { return _text; }
    };
}   //  !BnD

#endif  // !_B1SECS2_MESSAGE_S10F3_READABLE_H
