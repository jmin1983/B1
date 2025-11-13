//
// B1GEMServerSession.cpp
//
// Library: B1GEM
// Package: Server
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1GEM.h"
#include "B1GEMServerSession.h"
#include "B1GEMConsts.h"
#include "B1GEMDataRepository.h"
#include "B1GEMServer.h"

#include <B1Base/B1Time.h>
#include <B1SECS2/B1SECS2Data.h>

using namespace BnD;
using namespace B1GEMConsts;

B1GEMServerSession::B1GEMServerSession(B1ServerSocket* serverSocket, B1BaseServerSessionListener* listener, uint16 secs2SessionID, B1GEMServer* owner)
    : B1SECS2ServerSession(serverSocket, listener, secs2SessionID, owner)
    , _communicating(false)
    , _attemptOnline(false)
{
}

B1GEMServerSession::~B1GEMServerSession()
{
}

void B1GEMServerSession::onSelectCompleted(uint16 sessionID, const std::vector<uint8>& systemBytes)
{
    writeLog("onSelectCompleted");
}

bool B1GEMServerSession::onRecvHSMSData(uint8 stream, uint8 function)
{
    if (1 == stream && (13 == function || 14 == function)) {
        return true;    //  S1F13 is always allowed.
    }
    if (isCommunicating() != true) {
        writeLog("Communications State is NOT COMMUNICATING");
        return false;
    }
    auto controlState = owner()->controlState();
    switch (controlState) {
        default:
        case CONTROL_STATE_OFFLINE_EQ:
            return onRecvHSMSDataControlStateOfflineEQ(stream, function);
        case CONTROL_STATE_OFFLINE_HOST:
            return onRecvHSMSDataControlStateOfflineHost(stream, function);
        case CONTROL_STATE_ONLINE_LOCAL:
            return onRecvHSMSDataControlStateOnlineLocal(stream, function);
        case CONTROL_STATE_ONLINE_REMOTE:
            break;
    }    
    return true;
}

void B1GEMServerSession::onRecvMessageS1F0(uint16 sessionID, const std::vector<uint8>& systemBytes)
{
    if (_attemptOnline) {
        owner()->setControlState(CONTROL_STATE_OFFLINE_HOST);
        _attemptOnline = false;
    }
}

void B1GEMServerSession::onRecvMessageS1F1(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes)
{
    auto controlState = owner()->controlState();
    if (CONTROL_STATE_ONLINE_LOCAL == controlState ||
        CONTROL_STATE_ONLINE_REMOTE == controlState) {
        sendMessageS1F2(sessionID, systemBytes, owner()->modelType(), owner()->softwareRev());
    }
    else {
        sendMessageS1F0(sessionID, systemBytes);
    }
}

void B1GEMServerSession::onRecvMessageS1F2(uint16 sessionID, const std::vector<uint8>& systemBytes)
{
    if (_attemptOnline) {
        sendCurrentControlState();
        _attemptOnline = false;
    }
}

void B1GEMServerSession::onRecvMessageS1F3(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const std::vector<B1SECS2DataSVID>& svIDs)
{
    auto svs = dataRepository()->getStatusVariableData(svIDs);
    sendMessageS1F4(sessionID, systemBytes, svs);
}

void B1GEMServerSession::onRecvMessageS1F11(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const std::vector<B1SECS2DataSVID>& svIDs)
{
    auto svs = dataRepository()->findStatusVariables(svIDs);
    sendMessageS1F12(sessionID, systemBytes, svs);
}

void B1GEMServerSession::onRecvMessageS1F13(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes)
{
    bool setCommunicatingResult = true; //  set false if job failed.
    writeLog("Communications State is set to %s", setCommunicatingResult ? "COMMUNICATING" : "COMM FAILED");
    if (setCommunicatingResult) {
        owner()->setCommStateCommunicating();
    }
    _communicating = setCommunicatingResult;
    implSendMessageS1F14(sessionID, systemBytes, setCommunicatingResult);
    sendCurrentControlState();
}

void B1GEMServerSession::onRecvMessageS1F14(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataCOMMACK& commack)
{
    std::shared_ptr<B1SECS2DataBIN> bin(static_cast<B1SECS2DataBIN*>(commack.get()->clone()));
    if (bin->data()[0] == 1) {
        disconnect();
    }
}

void B1GEMServerSession::onRecvMessageS1F15(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes)
{
    auto controlState = owner()->controlState();
    if (CONTROL_STATE_OFFLINE_EQ == controlState) {
        writeLog("Host Requested Offline but Control State is OFFLINE EQ -> deny host request");
        sendMessageS1F0(sessionID, systemBytes);
    }
    else {
        writeLog("Control State is set to OFFLINE HOST");
        owner()->setControlState(CONTROL_STATE_OFFLINE_HOST);
        implSendMessageS1F16(sessionID, systemBytes);
        sendCurrentControlState();
    }
}

void B1GEMServerSession::onRecvMessageS1F17(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes)
{
    implSendMessageS1F18(sessionID, systemBytes, owner()->controlState());
}

void B1GEMServerSession::onRecvMessageS2F13(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const std::vector<B1SECS2DataECID>& ecIDs)
{
    auto ecvs = dataRepository()->getEquipmentConstantValue(ecIDs);
    sendMessageS2F14(sessionID, systemBytes, ecvs);
}

void B1GEMServerSession::onRecvMessageS2F15(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const std::map<B1SECS2DataECID, B1SECS2DataECV>& ecvs)
{
    auto result = dataRepository()->changeEquipmentConstants(ecvs);
    auto eac = owner()->toEAC(result);
    sendMessageS2F16(sessionID, systemBytes, eac);
}

void B1GEMServerSession::onRecvMessageS2F17(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes)
{
    sendMessageS2F18(sessionID, systemBytes, owner()->currentTime());
}

void B1GEMServerSession::onRecvMessageS2F29(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const std::vector<B1SECS2DataECID>& ecIDs)
{
    std::vector<B1SECS2DataECID> resultECIDs;
    std::vector<B1SECS2DataECNAME> ecNames;
    std::vector<B1SECS2DataECMIN> ecMins;
    std::vector<B1SECS2DataECMAX> ecMaxes;
    std::vector<B1SECS2DataECDEF> ecDefs;
    std::vector<B1SECS2DataUNITS> units;
    dataRepository()->findEquipmentConstants(ecIDs, &resultECIDs, &ecNames, &ecMins, &ecMaxes, &ecDefs, &units);
    sendMessageS2F30(sessionID, systemBytes, resultECIDs, ecNames, ecMins, ecMaxes, ecDefs, units);
}

void B1GEMServerSession::onRecvMessageS2F31(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const B1SECS2DataTIME& time)
{
    bool result = owner()->implSyncSystemTime(time);
    implSendMessageS2F32(sessionID, systemBytes, result);
}

void B1GEMServerSession::onRecvMessageS2F33(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes,
                                          const B1SECS2DataDATAID& dataID, const std::map<B1SECS2DataRPTID, std::vector<B1SECS2DataVID> >& reportData)
{
    auto result = dataRepository()->updateReport(reportData);
    if (B1GEMReportRepository::RESULT_ACCEPTED == result && reportData.empty()) {
        dataRepository()->deleteAllReports();
    }
    auto drack = owner()->toDRACK(result);
    sendMessageS2F34(sessionID, systemBytes, drack);
}

void B1GEMServerSession::onRecvMessageS2F35(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes,
                                          const B1SECS2DataDATAID& dataID, const std::map<B1SECS2DataCEID, std::vector<B1SECS2DataRPTID> >& linkEventData)
{
    auto result = dataRepository()->linkReport(linkEventData);
    auto lrack = owner()->toLRACK(result);
    sendMessageS2F36(sessionID, systemBytes, lrack);
}

void B1GEMServerSession::onRecvMessageS6F15(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const B1SECS2DataCEID& ceID)
{
    std::map<B1SECS2DataRPTID, std::list<B1SECS2DataV> > eventReportData;
    if (dataRepository()->getEventReportData(ceID, B1GEMDataValueCondition(), &eventReportData)) {
        sendMessageS6F16(sessionID, systemBytes, owner()->nextDataID(), ceID, eventReportData);
    }
}

B1GEMServer* B1GEMServerSession::owner() const
{
    return static_cast<B1GEMServer*>(_owner);
}

B1GEMDataRepository* B1GEMServerSession::dataRepository()
{
    return owner()->dataRepository();
}

bool B1GEMServerSession::onRecvHSMSDataControlStateOfflineEQ(uint8 stream, uint8 function)
{
    bool result = false;
    switch (stream) {   //  allow meesage when eq_control_state == eq_offline.
        case 1:
            result = 1 == function || 17 == function;
            break;
        case 6:
            result = 12 == function;
            break;
        default:
            break;
    }
    if (result != true) {
        writeLog("not allowed in control_state is offline_eq");
    }
    return result;
}

bool B1GEMServerSession::onRecvHSMSDataControlStateOfflineHost(uint8 stream, uint8 function)
{
    bool result = false;
    switch (stream) {   //  allow meesage when eq_control_state == host_offline.
        case 1:
            result = 1 == function || 17 == function;
            break;
        case 6:
            result = 12 == function;
            break;
        default:
            break;
    }
    if (result != true) {
        writeLog("not allowed in control_state is offline_host");
    }
    return result;
}

bool B1GEMServerSession::onRecvHSMSDataControlStateOnlineLocal(uint8 stream, uint8 function)
{
    return true;    //  allow all message when eq_control_state == online_local(same as online_remote).
}
