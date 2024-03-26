//
// B1SECS2MessageS9.h
//
// Library: B1SECS2
// Package: S9
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_MESSAGE_S9_WRITABLE_H
#define _B1SECS2_MESSAGE_S9_WRITABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2MessageWritable.h>

namespace BnD {
    class B1SECS2MessageS9 : public B1SECS2MessageWritable {
    public:
        B1SECS2MessageS9(uint8 function, B1String&& messageName, const std::vector<uint8>& messageHeader);
        virtual ~B1SECS2MessageS9() {}
    protected:
        const std::shared_ptr<B1SECS2DataBIN> _messageHeaderBin;    //  size:10
    protected:
        virtual bool implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data) override;
    };

#define DEFINE_SECS2_MESSAGE_FUNCTION_S9(func, msg, name)\
    class B1SECS2MessageS9F##func : public B1SECS2MessageS9 {\
    public:\
        B1SECS2MessageS9F##func(const std::vector<uint8>& messageHeader) : B1SECS2MessageS9(func, msg, messageHeader) {}\
    protected:\
        virtual B1String fullName() override { return name; }\
    };
    DEFINE_SECS2_MESSAGE_FUNCTION_S9(1,     "sendMessageS9F1",  "Unrecognized Device ID (UDN)");
    DEFINE_SECS2_MESSAGE_FUNCTION_S9(3,     "sendMessageS9F3",  "Unrecognized Stream Type (USN)");
    DEFINE_SECS2_MESSAGE_FUNCTION_S9(5,     "sendMessageS9F5",  "Unrecognized Function Type (UFN)");
    DEFINE_SECS2_MESSAGE_FUNCTION_S9(7,     "sendMessageS9F7",  "Illegal Data (IDN)");
    DEFINE_SECS2_MESSAGE_FUNCTION_S9(9,     "sendMessageS9F9",  "Transaction Timer Timeout (TTN)");
    DEFINE_SECS2_MESSAGE_FUNCTION_S9(11,    "sendMessageS9F11", "Data Too Long (DLN)");
}   //  !BnD

#endif  // !_B1SECS2_MESSAGE_S9_WRITABLE_H
