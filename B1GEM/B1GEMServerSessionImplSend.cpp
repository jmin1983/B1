//
// B1GEMServerSessionImplSend.cpp
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
#include "B1GEMAlarm.h"
#include "B1GEMServer.h"
#include "B1GEMDataRepository.h"
#include "B1GEMDataValueCondition.h"

#include <B1SECS2/B1SECS2Data.h>

using namespace BnD;
using namespace B1GEMConsts;

void B1GEMServerSession::sendCurrentControlState()
{
    auto controlState = owner()->controlState();
    sendEvent(owner()->toCEID(controlState), B1GEMDataValueCondition());
}

void B1GEMServerSession::sendControlStateOffline()
{
    sendEvent(owner()->toCEID(CONTROL_STATE_OFFLINE_EQ), B1GEMDataValueCondition());
}

void B1GEMServerSession::sendControlStateAttemptOnline()
{
    _attemptOnline = true;
    sendMessageS1F1(_secs2SessionID, makeNewSystemBytes());
}

void B1GEMServerSession::sendAlarm(const B1SECS2DataALID& alID, bool isSetAlarm, bool sendRelatedCEID, const B1GEMDataValueCondition& gemCondition)
{
    if (isAlarmEnabled(alID) != true) {
        writeLog("skip sendAlarm. alarm disabled: [%s]", alID->toString().cString());
        return;
    }
    auto alarm = owner()->dataRepository()->findAlarm(alID, isSetAlarm);
    if (alarm == NULL) {
        writeLog("sendAlarm failed. can not find alID: [%s]", alID->toString().cString());
        return;
    }
    if (isCommunicating() != true) {
        writeLog("sendAlarm requested but NOT COMMUNICATING: [%s]", alID->toString().cString());
        return;
    }
    sendMessageS5F1(_secs2SessionID, makeNewSystemBytes(), alarm->alCD(), alarm->alID(), alarm->alTX());
    if (sendRelatedCEID) {
        sendEvent(alarm->ceID(), gemCondition);
    }
}

void B1GEMServerSession::sendEvent(const B1SECS2DataCEID& ceID, const B1GEMDataValueCondition& gemCondition)
{
    if (isEventEnabled(ceID) != true) {
        writeLog("skip sendEvent. event disabled: ceID[%s]", ceID->toString().cString());
        return;
    }
    if (isCommunicating() != true) {
        writeLog("sendEvent requested but NOT COMMUNICATING: ceID[%s]", ceID->toString().cString());
        return;
    }
    std::map<B1SECS2DataRPTID, std::list<B1SECS2DataV> > eventReportData;
    if (owner()->dataRepository()->getEventReportData(ceID, gemCondition, &eventReportData)) {
        sendMessageS6F11(_secs2SessionID, makeNewSystemBytes(), owner()->nextDataID(), ceID, eventReportData);
    }
}
