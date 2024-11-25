//
// B1SMTPPacketMaker.h
//
// Library: B1SMTP
// Package: B1SMTP
// Module:  B1SMTP
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SMTP_SMTP_PACKET_MAKER_H
#define _B1SMTP_SMTP_PACKET_MAKER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SMTP/B1Mail.h>

namespace BnD {
    class B1SMTPPacketMaker {
    public:
        B1SMTPPacketMaker();
        virtual ~B1SMTPPacketMaker();
    protected:
        std::vector<uint8> makeRequestMessage(const B1String& message);
        B1String makeParticipantsString(const std::list<B1Mail::B1Participant>& participants) const;
    public:
        std::vector<uint8> makeDataHello(const B1String& serverName, bool useAuth = false);
        std::vector<uint8> makeDataMailFrom(const B1String& mailAddress);
        std::vector<uint8> makeDataRcptTO(const B1String& mailAddress);
        std::vector<uint8> makeDataRcptCC(const B1String& mailAddress);
        std::vector<uint8> makeDataRcptBCC(const B1String& mailAddress);
        std::vector<uint8> makeDataStartMailInput();
        std::vector<uint8> makeDataContents(const B1Mail& mail);
        std::vector<uint8> makeDataQuit();
    };
}   //  !BnD

#endif  // !_B1SMTP_SMTP_PACKET_MAKER_H
