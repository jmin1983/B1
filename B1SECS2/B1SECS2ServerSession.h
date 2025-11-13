//
// B1SECS2ServerSession.h
//
// Library: B1SECS2
// Package: Server
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_SERVER_SESSION_H
#define _B1SECS2_SERVER_SESSION_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif
 
#include <B1Network/B1ArrayBufferServerSession.h>
#include <B1SECS2/B1HSMSMessage.h>
#include <B1SECS2/B1SECS2Consts.h>
#include <B1SECS2/B1SECS2DataManager.h>
#include <B1Util/B1TimeChecker.h>

namespace BnD {
    class B1SECS2MessageWritable;
    class B1SECS2Server;
    class B1SECS2ServerSession : public B1ArrayBufferServerSession {
    public:
        B1SECS2ServerSession(B1ServerSocket* serverSocket, B1BaseServerSessionListener* listener, uint16 secs2SessionID, B1SECS2Server* owner);
        virtual ~B1SECS2ServerSession();
    private:
        int32 _initialSystemByte;
    protected:
        uint16 _secs2SessionID; //  deviceID
        B1SECS2Server* _owner;
        B1TimeChecker _t6Checker;
        B1TimeChecker _t7Checker;
        B1TimeChecker _t8Checker;
        bool _selected; //  is HSMS.
        B1HSMSMessage _lastMessage;
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
        void recvMessageS1F3(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS1F11(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS1F13(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS1F14(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS1F15(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS1F17(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS1F21(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS1F23(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS2F0(uint16 sessionID, const std::vector<uint8>& systemBytes);
        void recvMessageS2F13(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS2F15(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS2F17(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);        
        void recvMessageS2F29(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS2F31(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS2F33(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS2F35(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS2F37(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS2F41(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS2F49(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS5F0(uint16 sessionID, const std::vector<uint8>& systemBytes);
        void recvMessageS5F2(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS5F3(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS5F5(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS5F7(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS6F0(uint16 sessionID, const std::vector<uint8>& systemBytes);
        void recvMessageS6F12(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS6F15(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS6F19(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void recvMessageS64F0(uint16 sessionID, const std::vector<uint8>& systemBytes);
        void recvMessageS64F1(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength);
        void makeMessageHeader(uint16 sessionID, uint8 stream, uint8 function, bool wait, uint8 stype, const std::vector<uint8>& systemBytes, std::vector<uint8>* result);
    protected:
        void writeLog(const char* format, ...);
        const B1SECS2DataManager* secs2DataManager() const;
        std::vector<uint8> makeNewSystemBytes();
        bool toUint32FromSystemBytes(const std::vector<uint8>& systemBytes, int32* result) const;   //  return false if converting failed.
        void sendControlMessage(uint16 sessionID, const std::vector<uint8>& systemBytes, B1SECS2Consts::CONTROL_MESSAGE controlMessage, bool wait = false);
        void sendDataMessage(uint16 sessionID, const std::vector<uint8>& systemBytes, uint8 stream, uint8 function, bool wait);
        void sendDataMessage(uint16 sessionID, const std::vector<uint8>& systemBytes, uint8 stream, uint8 function, const std::vector<std::vector<uint8> >& items, bool wait = false);
        void sendDataMessage(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2MessageWritable& message);
        void sendMessageF0(uint8 stream, uint16 sessionID, const std::vector<uint8>& systemBytes);
        void sendMessageS1F0(uint16 sessionID, const std::vector<uint8>& systemBytes) { sendMessageF0(1, sessionID, systemBytes); }
        void sendMessageS1F1(uint16 sessionID, const std::vector<uint8>& systemBytes);
        void sendMessageS1F2(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataMDLN& mdln, const B1SECS2DataSOFTREV& softRev);
        void sendMessageS1F4(uint16 sessionID, const std::vector<uint8>& systemBytes, const std::list<B1SECS2DataSV>& svs);
        void sendMessageS1F12(uint16 sessionID, const std::vector<uint8>& systemBytes,
                              const std::map<B1SECS2DataSVID, std::pair<B1SECS2DataSVNAME, B1SECS2DataUNITS> >& svs);
        void sendMessageS1F13(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataMDLN& mdln, const B1SECS2DataSOFTREV& softRev);
        void sendMessageS1F14(uint16 sessionID, const std::vector<uint8>& systemBytes,
                              const B1SECS2DataCOMMACK& commAck, const B1SECS2DataMDLN& mdln, const B1SECS2DataSOFTREV& softRev);
        void sendMessageS1F16(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataOFLACK& oflAck);
        void sendMessageS1F18(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataONLACK& onlAck);
        void sendMessageS1F22(uint16 sessionID, const std::vector<uint8>& systemBytes,
                              const std::map<B1SECS2DataVID, std::pair<B1SECS2DataDVVALNAME, B1SECS2DataUNITS> >& vNames);
        void sendMessageS1F24(uint16 sessionID, const std::vector<uint8>& systemBytes,
                              const std::map<B1SECS2DataCEID, std::pair<B1SECS2DataCENAME, std::list<B1SECS2DataVID> > >& ceNames);
        void sendMessageS2F0(uint16 sessionID, const std::vector<uint8>& systemBytes) { sendMessageF0(2, sessionID, systemBytes); }
        void sendMessageS2F14(uint16 sessionID, const std::vector<uint8>& systemBytes, const std::list<B1SECS2DataECV>& data);
        void sendMessageS2F16(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataEAC& eac);
        void sendMessageS2F18(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataTIME& time);
        void sendMessageS2F30(uint16 sessionID, const std::vector<uint8>& systemBytes,
                              const std::vector<B1SECS2DataECID>& ecIDs, const std::vector<B1SECS2DataECNAME>& ecNames, const std::vector<B1SECS2DataECMIN>& ecMins,
                              const std::vector<B1SECS2DataECMAX>& ecMaxes, const std::vector<B1SECS2DataECDEF>& ecDefs, const std::vector<B1SECS2DataUNITS>& units);
        void sendMessageS2F32(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataTIACK& tiAck);
        void sendMessageS2F34(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataDRACK& drAck);
        void sendMessageS2F36(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataLRACK& lrAck);
        void sendMessageS2F38(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataERACK& erAck);
        void sendMessageS2F42(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataHCACK& hcAck, const std::map<B1SECS2DataCPNAME, B1SECS2DataCPACK>& cps);
        void sendMessageS2F50(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataHCACK& hcAck, const std::map<B1SECS2DataCPNAME, B1SECS2DataCEPACK>& cps);
        void sendMessageS5F0(uint16 sessionID, const std::vector<uint8>& systemBytes) { sendMessageF0(5, sessionID, systemBytes); }
        void sendMessageS5F1(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataALCD& alCD, const B1SECS2DataALID& alID, const B1SECS2DataALTX& alTX);
        void sendMessageS5F4(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataACKC5& ackc5);
        void sendMessageS5F6(uint16 sessionID, const std::vector<uint8>& systemBytes, const std::map<B1SECS2DataALID, std::pair<B1SECS2DataALCD, B1SECS2DataALTX> >& alarms);
        void sendMessageS5F8(uint16 sessionID, const std::vector<uint8>& systemBytes, const std::map<B1SECS2DataALID, std::pair<B1SECS2DataALCD, B1SECS2DataALTX> >& alarms);
        void sendMessageS6F0(uint16 sessionID, const std::vector<uint8>& systemBytes) { sendMessageF0(6, sessionID, systemBytes); }
        void sendMessageS6F11(uint16 sessionID, const std::vector<uint8>& systemBytes,
                              const B1SECS2DataDATAID& dataID, const B1SECS2DataCEID& ceID, const std::map<B1SECS2DataRPTID, std::list<B1SECS2DataV> >& reportsData);
        void sendMessageS6F16(uint16 sessionID, const std::vector<uint8>& systemBytes,
                              const B1SECS2DataDATAID& dataID, const B1SECS2DataCEID& ceID, const std::map<B1SECS2DataRPTID, std::list<B1SECS2DataV> >& reportsData);
        void sendMessageS6F20(uint16 sessionID, const std::vector<uint8>& systemBytes, const std::list<B1SECS2DataV>& reportData);
        void sendMessageS9F1(uint16 sessionID, const std::vector<uint8>& systemBytes, const std::vector<uint8>& messageHeader);
        void sendMessageS9F3(uint16 sessionID, const std::vector<uint8>& systemBytes, const std::vector<uint8>& messageHeader);
        void sendMessageS9F5(uint16 sessionID, const std::vector<uint8>& systemBytes, const std::vector<uint8>& messageHeader);
        void sendMessageS9F7(uint16 sessionID, const std::vector<uint8>& systemBytes, const std::vector<uint8>& messageHeader);
        void sendMessageS9F9(uint16 sessionID, const std::vector<uint8>& systemBytes, const std::vector<uint8>& messageHeader);
        void sendMessageS9F11(uint16 sessionID, const std::vector<uint8>& systemBytes, const std::vector<uint8>& messageHeader);
        void sendMessageS64F0(uint16 sessionID, const std::vector<uint8>& systemBytes) { sendMessageF0(64, sessionID, systemBytes); }
        void sendMessageS64F2(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataHCACK& hcAck, const std::map<B1SECS2DataCPNAME, B1SECS2DataCPACK>& cps);
    protected:
        virtual void onSelectCompleted(uint16 sessionID, const std::vector<uint8>& systemBytes) {}
        virtual void onDeselectCompleted() {}
        virtual bool onRecvHSMSData(uint8 stream, uint8 function) { return true; }  //  return false to send F0
        virtual void onRecvMessageS1F0(uint16 sessionID, const std::vector<uint8>& systemBytes) {}
        virtual void onRecvMessageS1F1(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes);
        virtual void onRecvMessageS1F2(uint16 sessionID, const std::vector<uint8>& systemBytes);
        virtual void onRecvMessageS1F3(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const std::vector<B1SECS2DataSVID>& svIDs);
        virtual void onRecvMessageS1F11(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const std::vector<B1SECS2DataSVID>& svIDs);
        virtual void onRecvMessageS1F13(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes);
        virtual void onRecvMessageS1F14(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataCOMMACK& commack);
        virtual void onRecvMessageS1F15(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes);
        virtual void onRecvMessageS1F17(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes);
        virtual void onRecvMessageS1F21(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const std::vector<B1SECS2DataVID>& vIDs);
        virtual void onRecvMessageS1F23(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const std::vector<B1SECS2DataCEID>& ceIDs);
        virtual void onRecvMessageS2F0(uint16 sessionID, const std::vector<uint8>& systemBytes) {}
        virtual void onRecvMessageS2F13(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const std::vector<B1SECS2DataECID>& ecIDs);
        virtual void onRecvMessageS2F15(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const std::map<B1SECS2DataECID, B1SECS2DataECV>& ecvs);
        virtual void onRecvMessageS2F17(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes);
        virtual void onRecvMessageS2F29(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const std::vector<B1SECS2DataECID>& ecIDs);
        virtual void onRecvMessageS2F31(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const B1SECS2DataTIME& time);
        virtual void onRecvMessageS2F33(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes,
                                        const B1SECS2DataDATAID& dataID, const std::map<B1SECS2DataRPTID, std::vector<B1SECS2DataVID> >& reportData);
        virtual void onRecvMessageS2F35(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes,
                                        const B1SECS2DataDATAID& dataID, const std::map<B1SECS2DataCEID, std::vector<B1SECS2DataRPTID> >& linkEventData);
        virtual void onRecvMessageS2F37(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes,
                                        const B1SECS2DataCEED& ceed, const std::vector<B1SECS2DataCEID>& ceIDs);
        virtual void onRecvMessageS2F41(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes,
                                        const B1SECS2DataRCMD& rcmd, const std::map<B1SECS2DataCPNAME, B1SECS2DataCPVAL>& cps);
        virtual void onRecvMessageS2F49(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes,
                                        const B1SECS2DataDATAID& dataID, const B1SECS2DataRCMD& rcmd, const std::map<B1SECS2DataCPNAME, B1SECS2DataCEPVAL>& cps);
        virtual void onRecvMessageS5F0(uint16 sessionID, const std::vector<uint8>& systemBytes) {}
        virtual void onRecvMessageS5F2(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataACKC5& ackc5);
        virtual void onRecvMessageS5F3(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const B1SECS2DataALED& alED, const B1SECS2DataALID& alID);
        virtual void onRecvMessageS5F5(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const B1SECS2DataALID& alID);
        virtual void onRecvMessageS5F7(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes);
        virtual void onRecvMessageS6F0(uint16 sessionID, const std::vector<uint8>& systemBytes) {}
        virtual void onRecvMessageS6F12(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataACKC6& ackc6);
        virtual void onRecvMessageS6F15(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const B1SECS2DataCEID& ceID);
        virtual void onRecvMessageS6F19(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const B1SECS2DataRPTID& rptID);
        virtual void onRecvMessageS64F0(uint16 sessionID, const std::vector<uint8>& systemBytes) {}
        virtual void onRecvMessageS64F1(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes,
                                        const B1SECS2DataRCMD& rcmd, const std::map<B1SECS2DataCPNAME, B1SECS2DataCPVAL>& cps);
    protected:
        virtual void implOnConnect() override;
        virtual void onReadComplete(uint8* data, size_t dataSize) override;
        virtual void implProcessConnected(bool firstConnectedProcess) override;
    };
}   //  !BnD

#endif  // !_B1SECS2_SERVER_SESSION_H
