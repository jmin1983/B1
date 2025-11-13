//
// B1GEMServer.h
//
// Library: B1GEM
// Package: Server
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1GEM_SERVER_H
#define _B1GEM_SERVER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2DataTypes.h>
#include <B1SECS2/B1SECS2Server.h>
#include <B1Util/B1LockUtil.h>

#include <B1GEM/B1GEMConsts.h>
#include <B1GEM/B1GEMEventRepository.h>
#include <B1GEM/B1GEMReportRepository.h>

namespace BnD {
    struct B1GEMDataValueCondition;
    class B1GEMDataRepository;
    class B1GEMServerSessionManager;
    class B1GEMServer : public B1SECS2Server {
    public:
        B1GEMServer(uint16 deviceID, B1GEMConsts::COMM_STATE initialCommState, B1GEMConsts::CONTROL_STATE initialControlState);
        virtual ~B1GEMServer();
    protected:
        mutable B1LockUtil<B1GEMConsts::COMM_STATE> _commState;
        mutable B1LockUtil<B1GEMConsts::CONTROL_STATE> _controlState;
        std::shared_ptr<B1GEMDataRepository> _dataRepository;
    protected:
        virtual B1BaseSessionManager* createSessionManager() override;
        bool isAcceptableSession(B1ServerSocket* serverSocket) final;
        void onServerSessionDisconnected(B1ServerSocket* serverSocket, int32 reason) final;
    protected:
        virtual bool implInitialize(int32 port) override;
        virtual void implFinalize() override;
    protected:
        virtual B1GEMDataRepository* createDataRepository() = 0;
        virtual void onCommStateChanged(B1GEMConsts::COMM_STATE newState) const {}
        virtual void onControlStateChanged(B1GEMConsts::CONTROL_STATE newState) const {}
    protected:
        B1GEMServerSessionManager* sessionManager() const;
    public:
        virtual B1SECS2DataDATAID nextDataID() const = 0;
        virtual B1SECS2DataCEID toCEID(B1GEMConsts::CONTROL_STATE controlState) const = 0;
        virtual B1SECS2DataDRACK toDRACK(B1GEMReportRepository::RESULT result) const = 0;
        virtual B1SECS2DataEAC toEAC(B1GEMEventRepository::RESULT result) const = 0;
        virtual B1SECS2DataLRACK toLRACK(B1GEMEventRepository::RESULT result) const = 0;
        virtual B1SECS2DataTIME currentTime() const = 0;
        virtual B1SECS2DataMDLN modelType() const = 0;
        virtual B1SECS2DataSOFTREV softwareRev() const = 0;
        virtual bool implSyncSystemTime(const B1SECS2DataTIME& time) const = 0;
    public:
        void setCommStateEnable(bool flag);
        void setCommStateCommunicating() const;
        void setControlState(B1GEMConsts::CONTROL_STATE state) const;
        void getCommState(B1GEMConsts::COMM_STATE* state);
        bool isSessionEmpty() const;
        B1GEMConsts::CONTROL_STATE controlState() const;
        B1GEMDataRepository* dataRepository() const { return _dataRepository.get(); }
        void sendAlarm(const B1SECS2DataALID& alID, bool isSetAlarm, bool sendRelatedCEID, const B1GEMDataValueCondition& gemCondition);
        void sendEvent(const B1SECS2DataCEID& ceID, const B1GEMDataValueCondition& gemCondition);
    };
}   //  !BnD

#endif  // !_B1GEM_SERVER_H
