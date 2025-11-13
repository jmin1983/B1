//
// B1SECS2Client.h
//
// Library: B1SECS2
// Package: Client
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_CLIENT_H
#define _B1SECS2_CLIENT_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1BaseClient.h>

#include <list>
#include <map>
#include <vector>

namespace BnD {
    class B1SECS2ClientSession;
    class B1SECS2DataACKC5;
    class B1SECS2DataACKC6;
    class B1SECS2DataALED;
    class B1SECS2DataALID;
    class B1SECS2DataCEED;
    class B1SECS2DataCEID;
    class B1SECS2DataCEPVAL;
    class B1SECS2DataCPNAME;
    class B1SECS2DataCPVAL;
    class B1SECS2DataDATAID;
    class B1SECS2DataECID;
    class B1SECS2DataOBJSPEC;
    class B1SECS2DataRCMD;
    class B1SECS2DataRPTID;
    class B1SECS2DataSVID;
    class B1SECS2DataVID;
    class B1SECS2Client : protected B1BaseClient {
    public:
        B1SECS2Client();
        virtual ~B1SECS2Client();
    protected:
        std::shared_ptr<B1SECS2ClientSession> getSession(uint16 deviceID) const;
    public:
        bool initialize();
        void finalize();
        bool connect(const B1String& address, uint16 port, uint16 deviceID);
        void disconnect(uint16 deviceID);
        bool sendMessageS1F3(uint16 deviceID, const std::list<B1SECS2DataSVID>& svIDs);
        bool sendMessageS1F11(uint16 deviceID, const std::list<B1SECS2DataSVID>& svIDs);
        bool sendMessageS1F13(uint16 deviceID);
        bool sendMessageS1F14(uint16 deviceID, const class B1SECS2DataCOMMACK& commAck);
        bool sendMessageS1F15(uint16 deviceID);
        bool sendMessageS1F17(uint16 deviceID);
        bool sendMessageS1F21(uint16 deviceID, const std::list<B1SECS2DataVID>& vIDs);
        bool sendMessageS1F23(uint16 deviceID, const std::list<B1SECS2DataCEID>& ceIDs);
        bool sendMessageS2F13(uint16 deviceID, const std::list<B1SECS2DataECID>& ecIDs);
        bool sendMessageS2F15(uint16 deviceID, const std::map<B1SECS2DataECID, class B1SECS2DataECV>& ecvs);
        bool sendMessageS2F17(uint16 deviceID);
        bool sendMessageS2F29(uint16 deviceID, const std::list<B1SECS2DataECID>& ecIDs);
        bool sendMessageS2F31(uint16 deviceID, const class B1SECS2DataTIME& time);
        bool sendMessageS2F33(uint16 deviceID, const B1SECS2DataDATAID& dataID, const std::map<B1SECS2DataRPTID, std::list<B1SECS2DataVID> >& reportData);
        bool sendMessageS2F35(uint16 deviceID, const B1SECS2DataDATAID& dataID, const std::map<B1SECS2DataCEID, std::list<B1SECS2DataRPTID> >& linkEventData);
        bool sendMessageS2F37(uint16 deviceID, const B1SECS2DataCEED& ceed, const std::list<B1SECS2DataCEID>& ceIDs);
        bool sendMessageS2F41(uint16 deviceID, const B1SECS2DataRCMD& rcmd, const std::map<B1SECS2DataCPNAME, B1SECS2DataCPVAL>& cps);
        bool sendMessageS2F49(uint16 deviceID,
                              const B1SECS2DataDATAID& dataID, const B1SECS2DataOBJSPEC& objSpec, const B1SECS2DataRCMD& rcmd,
                              const std::map<B1SECS2DataCPNAME, B1SECS2DataCEPVAL>& cps);
        bool sendMessageS5F3(uint16 deviceID, const B1SECS2DataALED& alED, const B1SECS2DataALID& alID);
        bool sendMessageS5F5(uint16 deviceID, const B1SECS2DataALID& alID);
        bool sendMessageS5F7(uint16 deviceID);
        bool sendMessageS6F15(uint16 deviceID, const B1SECS2DataCEID& ceID);
        bool sendMessageS6F19(uint16 deviceID, const B1SECS2DataRPTID& rptID);
        bool sendMessageS64F1(uint16 deviceID, const B1SECS2DataRCMD& rcmd, const std::map<B1SECS2DataCPNAME, B1SECS2DataCPVAL>& cps);
    };
}   //  !BnD

#endif  // !_B1SECS2_CLIENT_H
