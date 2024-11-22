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

namespace BnD {
    class B1SMTPPacketMaker {
    public:
        B1SMTPPacketMaker();
        virtual ~B1SMTPPacketMaker();
    protected:
        std::vector<uint8> makeRequestMessage(const B1String& message);
    public:
        std::vector<uint8> makeDataHello(const B1String& serverName, bool useAuth = false);
    };
}   //  !BnD

#endif  // !_B1SMTP_SMTP_PACKET_MAKER_H
