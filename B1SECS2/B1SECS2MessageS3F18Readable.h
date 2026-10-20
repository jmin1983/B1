//
// B1SECS2MessageS3F18Readable.h
//
// Library: B1SECS2
// Package: S3
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_MESSAGE_S3F18_READABLE_H
#define _B1SECS2_MESSAGE_S3F18_READABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2MessageReadable.h>

namespace BnD {
    class B1SECS2MessageS3F18Readable : public B1SECS2MessageReadable {
    public:
        B1SECS2MessageS3F18Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager);
        virtual ~B1SECS2MessageS3F18Readable();
    protected:
        B1SECS2DataCAACK _acAck;
        std::map<B1SECS2DataERRCODE, B1SECS2DataERRTEXT> _errorText;
    protected:
        virtual B1String fullName() override;
        virtual void implToString(B1String* string) override;
        virtual bool implReadData() override;
    public:
        const B1SECS2DataCAACK& acAck() const { return _acAck; }
        const std::map<B1SECS2DataERRCODE, B1SECS2DataERRTEXT>& errorText() const { return _errorText; }
    };
}   //  !BnD

#endif  // !_B1SECS2_MESSAGE_S3F18_READABLE_H
