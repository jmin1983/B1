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
#include <B1SECS2/B1SECS2Data.h>
#include <B1SECS2/B1SECS2DataManager.h>
#include <B1Util/B1TimeChecker.h>

namespace BnD {
    class B1ClientSocket;
    class B1SECS2MessageWritable;
    class B1SECS2ClientSession : public B1ArrayBufferClientSession {
    public:
        B1SECS2ClientSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* listener, uint16 secs2SessionID, const B1SECS2DataManager* secs2DataManager);
    protected:
        enum TIME_OUT {
            TIME_OUT_T3 = 45 * 1000,    //  T3(Reply Timeout) range:1~120sec, default:45sec
            TIME_OUT_T6 = 5 * 1000,     //  T5(Connect Separation Timeout) range:1~240sec, default:5sec
            TIME_OUT_T7 = 10 * 1000,    //  T7(NOT SELECTED Timeout) range:1~240sec, default:10sec
            TIME_OUT_T8 = 5 * 1000,     //  T8(Network Intercharacter Timeout) range:1~120sec, default:5sec
            TIME_OUT_ALIVE = 121 * 1000,
        };
        enum CONSTS {
            CONSTS_REQ_ALIVE_INTERVAL = 59 * 1000,
        };
        struct HSMSMessage {
            HSMSMessage() : _recved(0), _length(0), _data() {}
            uint32 _recved;
            uint32 _length;
            std::vector<uint8> _data;   //  10 byte header included
            void reset()
            {
                _recved = 0;
                _length = 0;
                _data.clear();
            }
        };
    private:
        int32 _initialSystemByte;
    protected:
        const uint16 _secs2SessionID; //  deviceID
        const B1SECS2DataManager* _secs2DataManager;
        HSMSMessage _lastMessage;
        B1TimeChecker _t7Checker;
        B1TimeChecker _aliveChecker;
        uint64 _lastAliveRequestTick;
    private:
        void recvHSMSMessage(const std::vector<uint8>& data);   //  10 byte header included
        void recvHSMSControl(uint16 sessionID, const std::vector<uint8>& systemBytes, B1SECS2Consts::CONTROL_MESSAGE controlMessage);
        void recvHSMSData(uint16 sessionID, uint8 stream, uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength, const std::vector<uint8>& messageHeader);
        bool recvHSMSDataStream1(uint16 sessionID, uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        bool recvHSMSDataStream2(uint16 sessionID, uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        bool recvHSMSDataStream5(uint16 sessionID, uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        bool recvHSMSDataStream6(uint16 sessionID, uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        bool recvHSMSDataStream64(uint16 sessionID, uint8 function, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS1F0(uint16 sessionID, const std::vector<uint8>& systemBytes);
        void recvMessageS1F1(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS1F2(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS1F4(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS1F14(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS1F16(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS1F18(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS2F0(uint16 sessionID, const std::vector<uint8>& systemBytes);
        void recvMessageS2F32(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS2F34(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS2F36(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS2F38(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS2F42(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS2F50(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS5F1(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS5F4(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS6F11(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS64F2(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void makeMessageHeader(uint16 sessionID, uint8 stream, uint8 function, bool wait, uint8 stype, const std::vector<uint8>& systemBytes, std::vector<uint8>* result);
    protected:
        const B1SECS2DataManager* secs2DataManager() const { return _secs2DataManager; }
        std::vector<uint8> makeNewSystemBytes();
        bool toUint32FromSystemBytes(const std::vector<uint8>& systemBytes, int32* result) const;   //  return false if converting failed.
        void sendControlMessage(uint16 sessionID, const std::vector<uint8>& systemBytes, B1SECS2Consts::CONTROL_MESSAGE controlMessage, bool wait = false);
        void sendDataMessage(uint16 sessionID, const std::vector<uint8>& systemBytes, uint8 stream, uint8 function, bool wait);
        void sendDataMessage(uint16 sessionID, const std::vector<uint8>& systemBytes, uint8 stream, uint8 function, const std::vector<std::vector<uint8> >& items, bool wait = false);
        void sendDataMessage(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2MessageWritable& message);
        void sendMessageF0(uint8 stream, uint16 sessionID, const std::vector<uint8>& systemBytes);
        void sendMessageS1F0(uint16 sessionID, const std::vector<uint8>& systemBytes) { sendMessageF0(1, sessionID, systemBytes); }
        bool sendMessageS1F2(uint16 sessionID, const std::vector<uint8>& systemBytes);
        bool sendMessageS5F2(uint16 sessionID, const B1SECS2DataACKC5& ackc5);
        bool sendMessageS6F12(uint16 sessionID, const B1SECS2DataACKC6& ackc6);
    protected:
        virtual void onRecvMessageS1F0(uint16 sessionID, const std::vector<uint8>& systemBytes) {}
        virtual void onRecvMessageS1F1(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes);
        virtual void onRecvMessageS1F2(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes) {}
        virtual void onRecvMessageS1F4(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const std::vector<B1SECS2DataSV>& svs) {}
        virtual void onRecvMessageS1F14(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes,
                                        const B1SECS2DataCOMMACK& commAck, const B1SECS2DataMDLN& mdln, const B1SECS2DataSOFTREV& softRev) {}
        virtual void onRecvMessageS1F16(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const B1SECS2DataOFLACK& oflAck) {}
        virtual void onRecvMessageS1F18(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const B1SECS2DataONLACK& onlAck) {}
        virtual void onRecvMessageS2F0(uint16 sessionID, const std::vector<uint8>& systemBytes) {}
        virtual void onRecvMessageS2F32(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const B1SECS2DataTIACK& tiAck) {}
        virtual void onRecvMessageS2F34(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const B1SECS2DataDRACK& drAck) {}
        virtual void onRecvMessageS2F36(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const B1SECS2DataLRACK& lrAck) {}
        virtual void onRecvMessageS2F38(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const B1SECS2DataERACK& erAck) {}
        virtual void onRecvMessageS2F42(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes,
                                        const B1SECS2DataHCACK& hcAck, const std::map<B1SECS2DataCPNAME, B1SECS2DataCPACK>& cps) {}
        virtual void onRecvMessageS2F50(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes,
                                        const B1SECS2DataHCACK& hcAck, const std::map<B1SECS2DataCPNAME, B1SECS2DataCEPACK>& cps) {}
        virtual void onRecvMessageS5F1(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes,
                                       const B1SECS2DataALCD& alcd, const B1SECS2DataALID& alid, const B1SECS2DataALTX& altx) {}
        virtual void onRecvMessageS5F4(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const B1SECS2DataACKC5& ackc5) {}
        virtual void onRecvMessageS6F11(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes,
                                        const B1SECS2DataDATAID dataID, const B1SECS2DataCEID& ceID, const std::map<B1SECS2DataRPTID, std::vector<B1SECS2DataV> >& reportData) {}
        virtual void onRecvMessageS64F2(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes,
                                        const B1SECS2DataHCACK& hcAck, const std::map<B1SECS2DataCPNAME, B1SECS2DataCPACK>& cps) {}
    protected:
        virtual void onSelectCompleted() {}
        virtual void onDeselectCompleted() {}
        virtual bool onRecvHSMSData(uint8 stream, uint8 function) { return true; }  //  return false to send F0
    protected:
        virtual void implOnConnect() override;
        virtual void onReadComplete(uint8* data, size_t dataSize) override;
        virtual void implProcessConnected(bool firstConnectedProcess) override;
    public:
        bool sendMessageS1F3(const std::list<B1SECS2DataSVID>& svIDs);
        bool sendMessageS1F13();
        bool sendMessageS1F15();
        bool sendMessageS1F17();
        bool sendMessageS2F31(const B1SECS2DataTIME& time);
        bool sendMessageS2F33(const B1SECS2DataDATAID& dataID, const std::map<B1SECS2DataRPTID, std::list<B1SECS2DataVID> >& reportData);
        bool sendMessageS2F35(const B1SECS2DataDATAID& dataID, const std::map<B1SECS2DataCEID, std::list<B1SECS2DataRPTID> >& linkEventData);
        bool sendMessageS2F37(const B1SECS2DataCEED& ceed, const std::list<B1SECS2DataCEID>& ceIDs);
        bool sendMessageS2F41(const B1SECS2DataRCMD& rcmd, const std::map<B1SECS2DataCPNAME, B1SECS2DataCPVAL>& cps);
        bool sendMessageS2F49(const B1SECS2DataDATAID& dataID, const B1SECS2DataOBJSPEC& objSpec, const B1SECS2DataRCMD& rcmd,
                              const std::map<B1SECS2DataCPNAME, B1SECS2DataCEPVAL>& cps);
        bool sendMessageS5F3(const B1SECS2DataALED& alED, const B1SECS2DataALID& alID);
        bool sendMessageS64F1(const B1SECS2DataRCMD& rcmd, const std::map<B1SECS2DataCPNAME, B1SECS2DataCPVAL>& cps);
    };
}   //  !BnD

#endif  // !_B1SECS2_CLIENT_SESSION_H
