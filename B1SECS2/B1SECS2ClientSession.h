//
// B1SECS2ClientSession.h
//
// Library: B1SECS2
// Package: Client
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_CLIENT_SESSION_H
#define _B1SECS2_CLIENT_SESSION_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1ArrayBufferClientSession.h>
#include <B1SECS2/B1HSMSMessage.h>
#include <B1SECS2/B1SECS2Data.h>
#include <B1SECS2/B1SECS2DataManager.h>
#include <B1Util/B1TimeChecker.h>

namespace BnD {
    class B1ClientSocket;
    class B1SECS2MessageWritable;
    class B1SECS2ClientSession : public B1ArrayBufferClientSession {
    public:
        B1SECS2ClientSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* listener);
    protected:
        enum CONSTS {
            CONSTS_REQ_ALIVE_INTERVAL = 59 * 1000,
            CONSTS_ALIVE_TIME_OUT = 121 * 1000,
        };
    private:
        int32 _initialSystemByte;
    protected:
        std::shared_ptr<B1SECS2DataManager> _secs2DataManager;
        B1HSMSMessage _lastMessage;
        B1TimeChecker _t7Checker;
        B1TimeChecker _aliveChecker;
        uint64 _lastAliveRequestTick;
    private:
        void recvHSMSMessage(const std::vector<uint8>& data);   //  10 byte header included
        void recvHSMSControl(uint16 sessionID, const std::vector<uint8>& systemBytes, B1SECS2Consts::CONTROL_MESSAGE controlMessage);
        void recvHSMSData(uint8 stream, uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength, const std::vector<uint8>& messageHeader);
        bool recvHSMSDataStream1(uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        bool recvHSMSDataStream2(uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        bool recvHSMSDataStream5(uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        bool recvHSMSDataStream6(uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        bool recvHSMSDataStream64(uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS1F0(const std::vector<uint8>& systemBytes);
        void recvMessageS1F1(bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS1F2(bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS1F4(bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS1F14(bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS1F16(bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS1F18(bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS2F0(const std::vector<uint8>& systemBytes);
        void recvMessageS2F32(bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS2F34(bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS2F36(bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS2F38(bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS2F42(bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS2F50(bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS5F1(bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS5F4(bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS6F11(bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS64F2(bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void makeMessageHeader(uint8 stream, uint8 function, bool wait, uint8 stype, const std::vector<uint8>& systemBytes, std::vector<uint8>* result);
    protected:
        virtual void onRecvMessageS1F0(const std::vector<uint8>& systemBytes) {}
        virtual void onRecvMessageS1F1(bool wait, const std::vector<uint8>& systemBytes);
        virtual void onRecvMessageS1F2(const std::vector<uint8>& systemBytes, const B1SECS2DataMDLN& mdln, const B1SECS2DataSOFTREV& softRev) {}
        virtual void onRecvMessageS1F4(const std::vector<uint8>& systemBytes, const std::vector<B1SECS2DataSV>& svs) {}
        virtual void onRecvMessageS1F14(const std::vector<uint8>& systemBytes,
                                        const B1SECS2DataCOMMACK& commAck, const B1SECS2DataMDLN& mdln, const B1SECS2DataSOFTREV& softRev) {}
        virtual void onRecvMessageS1F16(const std::vector<uint8>& systemBytes, const B1SECS2DataOFLACK& oflAck) {}
        virtual void onRecvMessageS1F18(const std::vector<uint8>& systemBytes, const B1SECS2DataONLACK& onlAck) {}
        virtual void onRecvMessageS2F0(const std::vector<uint8>& systemBytes) {}
        virtual void onRecvMessageS2F32(const std::vector<uint8>& systemBytes, const B1SECS2DataTIACK& tiAck) {}
        virtual void onRecvMessageS2F34(const std::vector<uint8>& systemBytes, const B1SECS2DataDRACK& drAck) {}
        virtual void onRecvMessageS2F36(const std::vector<uint8>& systemBytes, const B1SECS2DataLRACK& lrAck) {}
        virtual void onRecvMessageS2F38(const std::vector<uint8>& systemBytes, const B1SECS2DataERACK& erAck) {}
        virtual void onRecvMessageS2F42(const std::vector<uint8>& systemBytes,
                                        const B1SECS2DataHCACK& hcAck, const std::map<B1SECS2DataCPNAME, B1SECS2DataCPACK>& cps) {}
        virtual void onRecvMessageS2F50(const std::vector<uint8>& systemBytes,
                                        const B1SECS2DataHCACK& hcAck, const std::map<B1SECS2DataCPNAME, B1SECS2DataCEPACK>& cps) {}
        virtual void onRecvMessageS5F1(bool wait, const std::vector<uint8>& systemBytes,
                                       const B1SECS2DataALCD& alcd, const B1SECS2DataALID& alid, const B1SECS2DataALTX& altx) {}
        virtual void onRecvMessageS5F4(const std::vector<uint8>& systemBytes, const B1SECS2DataACKC5& ackc5) {}
        virtual void onRecvMessageS6F11(bool wait, const std::vector<uint8>& systemBytes,
                                        const B1SECS2DataDATAID dataID, const B1SECS2DataCEID& ceID, const std::map<B1SECS2DataRPTID, std::vector<B1SECS2DataV> >& reportData) {}
        virtual void onRecvMessageS64F2(const std::vector<uint8>& systemBytes,
                                        const B1SECS2DataHCACK& hcAck, const std::map<B1SECS2DataCPNAME, B1SECS2DataCPACK>& cps) {}
    protected:
        virtual std::shared_ptr<B1SECS2DataManager> createSECS2DataManager() = 0;
        virtual void onSelectCompleted() {}
        virtual void onDeselectCompleted() {}
        virtual bool onRecvHSMSData(uint8 stream, uint8 function) { return true; }  //  return false to send F0
    protected:
        virtual bool implInitializeSession() override;
        virtual void implFinalizeSession() override;
        virtual void implOnConnect() override;
        virtual void onReadComplete(uint8* data, size_t dataSize) override;
        virtual void implProcessConnected(bool firstConnectedProcess) override;
    protected:
        uint16 deviceID() const { return static_cast<uint16>(sessionHandleID()); }
        const B1SECS2DataManager* secs2DataManager() const { return _secs2DataManager.get(); }
        std::vector<uint8> makeNewSystemBytes();
        bool toUint32FromSystemBytes(const std::vector<uint8>& systemBytes, int32* result) const;   //  return false if converting failed.
        void sendControlMessage(const std::vector<uint8>& systemBytes, B1SECS2Consts::CONTROL_MESSAGE controlMessage, bool wait = false);
        void sendDataMessage(const std::vector<uint8>& systemBytes, uint8 stream, uint8 function, bool wait);
        void sendDataMessage(const std::vector<uint8>& systemBytes, uint8 stream, uint8 function, const std::vector<std::vector<uint8> >& items, bool wait = false);
        void sendDataMessage(const std::vector<uint8>& systemBytes, const B1SECS2MessageWritable& message);
        void sendMessageF0(uint8 stream, const std::vector<uint8>& systemBytes);
        void sendMessageS1F0(const std::vector<uint8>& systemBytes) { sendMessageF0(1, systemBytes); }
        bool sendMessageS1F1(const std::vector<uint8>& systemBytes);
        bool sendMessageS1F2(const std::vector<uint8>& systemBytes);
        bool sendMessageS5F2(const B1SECS2DataACKC5& ackc5);
        bool sendMessageS6F12(const B1SECS2DataACKC6& ackc6);
    public:
        bool sendMessageS1F3(const std::list<B1SECS2DataSVID>& svIDs);
        bool sendMessageS1F11(const std::list<B1SECS2DataSVID>& svIDs);
        bool sendMessageS1F13();
        bool sendMessageS1F14(const B1SECS2DataCOMMACK& commAck);
        bool sendMessageS1F15();
        bool sendMessageS1F17();
        bool sendMessageS1F21(const std::list<B1SECS2DataVID>& vIDs);
        bool sendMessageS1F23(const std::list<B1SECS2DataCEID>& ceIDs);
        bool sendMessageS2F13(const std::list<B1SECS2DataECID>& ecIDs);
        bool sendMessageS2F15(const std::map<B1SECS2DataECID, B1SECS2DataECV>& ecvs);
        bool sendMessageS2F17();
        bool sendMessageS2F29(const std::list<B1SECS2DataECID>& ecIDs);
        bool sendMessageS2F31(const B1SECS2DataTIME& time);
        bool sendMessageS2F33(const B1SECS2DataDATAID& dataID, const std::map<B1SECS2DataRPTID, std::list<B1SECS2DataVID> >& reportData);
        bool sendMessageS2F35(const B1SECS2DataDATAID& dataID, const std::map<B1SECS2DataCEID, std::list<B1SECS2DataRPTID> >& linkEventData);
        bool sendMessageS2F37(const B1SECS2DataCEED& ceed, const std::list<B1SECS2DataCEID>& ceIDs);
        bool sendMessageS2F41(const B1SECS2DataRCMD& rcmd, const std::map<B1SECS2DataCPNAME, B1SECS2DataCPVAL>& cps);
        bool sendMessageS2F49(const B1SECS2DataDATAID& dataID, const B1SECS2DataOBJSPEC& objSpec, const B1SECS2DataRCMD& rcmd,
                              const std::map<B1SECS2DataCPNAME, B1SECS2DataCEPVAL>& cps);
        bool sendMessageS5F3(const B1SECS2DataALED& alED, const B1SECS2DataALID& alID);
        bool sendMessageS5F5(const B1SECS2DataALID& alID);
        bool sendMessageS5F7();
        bool sendMessageS6F15(const B1SECS2DataCEID& ceID);
        bool sendMessageS6F19(const B1SECS2DataRPTID& rptID);
        bool sendMessageS64F1(const B1SECS2DataRCMD& rcmd, const std::map<B1SECS2DataCPNAME, B1SECS2DataCPVAL>& cps);
    };
}   //  !BnD

#endif  // !_B1SECS2_CLIENT_SESSION_H
