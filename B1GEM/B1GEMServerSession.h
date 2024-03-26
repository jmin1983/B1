//
// B1GEMServerSession.h
//
// Library: B1GEM
// Package: B1GEM
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1GEM_SERVER_SESSION_H
#define _B1GEM_SERVER_SESSION_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1GEM/B1GEMConsts.h>
#include <B1SECS2/B1SECS2ServerSession.h>

namespace BnD {
    struct B1GEMDataValueCondition;
    class B1GEMDataRepository;
    class B1GEMServer;
    class B1GEMServerSession : public B1SECS2ServerSession {
    public:
        B1GEMServerSession(B1ServerSocket* serverSocket, B1BaseServerSessionListener* listener, uint16 secs2SessionID, B1GEMServer* owner);
        virtual ~B1GEMServerSession();
    protected:
        bool _communicating;
        bool _attemptOnline;
    protected:
        virtual void onSelectCompleted(uint16 sessionID, const std::vector<uint8>& systemBytes) override;
        virtual bool onRecvHSMSData(uint8 stream, uint8 function) override;
        virtual void onRecvMessageS1F0(uint16 sessionID, const std::vector<uint8>& systemBytes) override;
        virtual void onRecvMessageS1F1(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes) override;
        virtual void onRecvMessageS1F2(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes) override;
        virtual void onRecvMessageS1F3(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const std::vector<B1SECS2DataSVID>& svIDs) override;
        virtual void onRecvMessageS1F11(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const std::vector<B1SECS2DataSVID>& svIDs) override;
                void onRecvMessageS1F13(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes) final;
        virtual void onRecvMessageS1F14(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const B1SECS2DataCOMMACK& commack) override;
                void onRecvMessageS1F15(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes) final;
                void onRecvMessageS1F17(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes) final;
        virtual void onRecvMessageS2F13(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const std::vector<B1SECS2DataECID>& ecIDs) override;
        virtual void onRecvMessageS2F15(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const std::map<B1SECS2DataECID, B1SECS2DataECV>& ecvs) override;
        virtual void onRecvMessageS2F17(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes) override;
        virtual void onRecvMessageS2F29(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const std::vector<B1SECS2DataECID>& ecIDs) override;
        virtual void onRecvMessageS2F31(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const B1SECS2DataTIME& time) override;
        virtual void onRecvMessageS2F33(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes,
                                        const B1SECS2DataDATAID& dataID, const std::map<B1SECS2DataRPTID, std::vector<B1SECS2DataVID> >& reportData) override;
        virtual void onRecvMessageS2F35(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes,
                                        const B1SECS2DataDATAID& dataID, const std::map<B1SECS2DataCEID, std::vector<B1SECS2DataRPTID> >& linkEventData) override;
        virtual void onRecvMessageS6F12(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const B1SECS2DataACKC6& ackc6) override {}
        virtual void onRecvMessageS6F15(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const B1SECS2DataCEID& ceID) override;
    protected:
        virtual bool isAlarmEnabled(const B1SECS2DataALID& alID) const { return true; }
        virtual bool isEventEnabled(const B1SECS2DataCEID& ceID) const { return true; }
        virtual void implSendMessageS1F13(uint16 sessionID, const std::vector<uint8>& systemBytes) = 0;
        virtual void implSendMessageS1F14(uint16 sessionID, const std::vector<uint8>& systemBytes, bool setCommunicatingResult) = 0;
        virtual void implSendMessageS1F16(uint16 sessionID, const std::vector<uint8>& systemBytes) = 0;
        virtual void implSendMessageS1F18(uint16 sessionID, const std::vector<uint8>& systemBytes, B1GEMConsts::CONTROL_STATE controlState) = 0;
        virtual void implSendMessageS2F32(uint16 sessionID, const std::vector<uint8>& systemBytes, bool result) = 0;
    protected:
        B1GEMServer* owner() const;
        B1GEMDataRepository* dataRepository();
        bool onRecvHSMSDataControlStateOfflineEQ(uint8 stream, uint8 function);     //  return false to stop transaction
        bool onRecvHSMSDataControlStateOfflineHost(uint8 stream, uint8 function);   //  return false to stop transaction
        bool onRecvHSMSDataControlStateOnlineLocal(uint8 stream, uint8 function);   //  return false to stop transaction
    public:
        bool isCommunicating() const { return _communicating; }
        bool isAttemptOnline() const { return _attemptOnline; }
        void sendCurrentControlState();
        void sendControlStateOffline();
        void sendControlStateAttemptOnline();
        void sendAlarm(const B1SECS2DataALID& alID, bool isSetAlarm, bool sendRelatedCEID, const B1GEMDataValueCondition& gemCondition);
        void sendEvent(const B1SECS2DataCEID& ceID, const B1GEMDataValueCondition& gemCondition);
    };
}   //  !BnD

#endif  // !_B1GEM_SERVER_SESSION_H
