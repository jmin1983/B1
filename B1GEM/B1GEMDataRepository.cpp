//
// B1GEMDataRepository.cpp
//
// Library: B1GEM
// Package: B1GEM
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1GEM.h"
#include "B1GEMDataRepository.h"
#include "B1GEMAlarmRepository.h"
#include "B1GEMVariableRepository.h"

#include <B1SECS2/B1SECS2Data.h>

using namespace BnD;

B1GEMDataRepository::B1GEMDataRepository()
    : _alarmRepository()
    , _eventRepository()
    , _variableRepository()
    , _reportRepository()
{
}

B1GEMDataRepository::~B1GEMDataRepository()
{
}

bool B1GEMDataRepository::initialize(const B1SECS2DataManager* secs2DataManager)
{
    _alarmRepository.reset(createAlarmRepository(secs2DataManager));
    _eventRepository.reset(createEventRepository(secs2DataManager));
    _variableRepository.reset(createVariableRepository(secs2DataManager));
    _reportRepository.reset(new B1GEMReportRepository());
    if (_alarmRepository == NULL ||
        _eventRepository == NULL ||
        _variableRepository == NULL ||
        _reportRepository == NULL) {
        _alarmRepository.reset();
        _eventRepository.reset();
        _variableRepository.reset();
        _reportRepository.reset();
        return false;
    }
    return true;
}

void B1GEMDataRepository::finalize()
{
    if (_reportRepository) {
        _reportRepository.reset();
    }
    if (_variableRepository) {
        _variableRepository->finalize();
        _variableRepository.reset();
    }
    if (_eventRepository) {
        _eventRepository->finalize();
        _eventRepository.reset();
    }
    if (_alarmRepository) {
        _alarmRepository->finalize();
        _alarmRepository.reset();
    }
}

SPB1GEMAlarm B1GEMDataRepository::findAlarm(const B1SECS2DataALID& alID, bool isSetAlarm) const
{
    return _alarmRepository->findAlarm(alID, isSetAlarm);
}

void B1GEMDataRepository::deleteAllReports()
{
    _eventRepository->deleteAllReports();
}

B1GEMEventRepository::RESULT B1GEMDataRepository::changeEquipmentConstants(const std::map<B1SECS2DataECID, B1SECS2DataECV>& ecs)
{
    bool result = false;
    for (const auto& ecsPair : ecs) {
        if (auto ec = _variableRepository->findEquipmentConstant(ecsPair.first)) {
            result = ec->modifyValue(ecsPair.second.data());
        }
    }

    if (result != true) {
        return B1GEMEventRepository::RESULT_BUSY;
    }
    else {
        return B1GEMEventRepository::RESULT_ACCEPTED;
    }
}

B1GEMEventRepository::RESULT B1GEMDataRepository::linkReport(const std::map<B1SECS2DataCEID, std::vector<B1SECS2DataRPTID> >& linkEventData)
{
    std::map<B1SECS2DataCEID, std::list<std::shared_ptr<B1SECS2DataRPTID> > > linkData;
    for (const auto& linkEventDataPair : linkEventData) {
        std::list<std::shared_ptr<B1SECS2DataRPTID> > rptIDs;
        for (const auto& rptID : linkEventDataPair.second) {
            rptIDs.push_back(std::make_shared<B1SECS2DataRPTID>(rptID));
        }
        linkData.insert(std::make_pair(linkEventDataPair.first, rptIDs));
    }
    return _eventRepository->linkReport(linkData);
}

bool B1GEMDataRepository::getAvailableCEIDs(const std::vector<B1SECS2DataCEID>& ceIDs, std::vector<B1SECS2DataCEID>* result) const
{
    return _eventRepository->getAvailableCEIDs(ceIDs, result);
}

SPGEMEvent B1GEMDataRepository::findEvent(const B1SECS2DataCEID& ceID) const
{
    return _eventRepository->findEvent(ceID);
}

void B1GEMDataRepository::findEquipmentConstants(const std::vector<B1SECS2DataECID>& ecIDs,
                                               std::vector<B1SECS2DataECID>* resultECIDs, std::vector<B1SECS2DataECNAME>* ecNames, std::vector<B1SECS2DataECMIN>* ecMins,
                                               std::vector<B1SECS2DataECMAX>* ecMaxes, std::vector<B1SECS2DataECDEF>* ecDefs, std::vector<B1SECS2DataUNITS>* units)
{
    resultECIDs->reserve(ecIDs.size());
    ecNames->reserve(ecIDs.size());
    ecMins->reserve(ecIDs.size());
    ecMaxes->reserve(ecIDs.size());
    ecDefs->reserve(ecIDs.size());
    units->reserve(ecIDs.size());
    for (const auto& ecID : ecIDs) {
        if (auto ec = _variableRepository->findEquipmentConstant(ecID)) {
            resultECIDs->push_back(ec->ecID());
            ecNames->push_back(ec->ecName());
            ecMins->push_back(ec->ecMin());
            ecMaxes->push_back(ec->ecMax());
            ecDefs->push_back(ec->ecDef());
            units->push_back(ec->units());
        }
        else {
            resultECIDs->push_back(ecID);
            ecNames->push_back(B1SECS2DataECNAME(SPB1SECS2Data(new B1SECS2DataASCII())));
            ecMins->push_back(B1SECS2DataECMIN(SPB1SECS2Data(new B1SECS2DataASCII())));
            ecMaxes->push_back(B1SECS2DataECMAX(SPB1SECS2Data(new B1SECS2DataASCII())));
            ecDefs->push_back(B1SECS2DataECDEF(SPB1SECS2Data(new B1SECS2DataASCII())));
            units->push_back(B1SECS2DataUNITS(SPB1SECS2Data(new B1SECS2DataASCII())));
        }
    }
}

SPB1GEMStatusVariable B1GEMDataRepository::findStatusVariable(const B1SECS2DataSVID& svID) const
{
    return _variableRepository->findStatusVariable(svID);
}

std::map<B1SECS2DataSVID, std::pair<B1SECS2DataSVNAME, B1SECS2DataUNITS> > B1GEMDataRepository::findStatusVariables(const std::vector<B1SECS2DataSVID>& svIDs) const
{
    return _variableRepository->findStatusVariables(svIDs);
}

B1GEMReportRepository::RESULT B1GEMDataRepository::updateReport(const std::map<B1SECS2DataRPTID, std::vector<B1SECS2DataVID> >& reportData)
{
    std::map<B1SECS2DataRPTID, std::list<std::shared_ptr<B1SECS2DataVID> > > newReports;
    for (const auto& reportDataPair : reportData) {
        std::list<std::shared_ptr<B1SECS2DataVID> > vIDs;
        for (const auto& vID : reportDataPair.second) {
            vIDs.push_back(std::make_shared<B1SECS2DataVID>(vID));
        }
        newReports.insert(std::make_pair(reportDataPair.first, vIDs));
    }
    return _reportRepository->updateReport(newReports);
}

SPGEMReport B1GEMDataRepository::findReport(const B1SECS2DataRPTID& rptID) const
{
    return _reportRepository->findReport(rptID);
}

std::vector<SPGEMReport> B1GEMDataRepository::findReports(const std::vector<std::shared_ptr<B1SECS2DataRPTID> >& rptIDs) const
{
    return _reportRepository->findReports(rptIDs);
}

bool B1GEMDataRepository::getEventReportData(const B1SECS2DataCEID& ceID, const B1GEMDataValueCondition& gemCondition,
                                           std::map<B1SECS2DataRPTID, std::list<B1SECS2DataV> >* result)
{
    auto evt = _eventRepository->findEvent(ceID);
    if (evt == NULL) {
        B1LOG("find event failed: [%s]", ceID->toString().cString());
        //assert(false);
        return false;
    }
    auto reports = _reportRepository->findReports(evt->reportIDs());
    if (reports.empty()) {
        return true;    //  no problem.
    }
    for (auto report : reports) {
        std::list<B1SECS2DataV> vs;
        for (auto vID : report->vIDs()) {
            if (auto v = _variableRepository->findVariable(*vID)) {
                if (auto secs2DataV = v->getValue(gemCondition)) {
                    vs.push_back(*secs2DataV);
                }
                else {
                    B1LOG("get v value failed. maybe no need to send event: CEID[%s], V[%s], condition:s",
                          ceID->toString().cString(), v->label().cString(), gemCondition.toString().cString());
                    return false;   //  no need to send event.
                }
            }
            else {
                vs.push_back(SPB1SECS2Data(new B1SECS2DataASCII()));
            }
        }
        result->insert(std::make_pair(report->rptID(), vs));
    }
    return true;
}

std::list<B1SECS2DataECV> B1GEMDataRepository::getEquipmentConstantValue(const std::vector<B1SECS2DataECID>& ecIDs)
{
    std::list<B1SECS2DataECV> result;
    for (const auto& ecID : ecIDs) {
        if (auto ec = _variableRepository->findEquipmentConstant(ecID)) {
            result.push_back(ec->getValue());
        }
        else {
            result.push_back(SPB1SECS2Data(new B1SECS2DataASCII()));
        }
    }
    return result;
}

std::list<B1SECS2DataSV> B1GEMDataRepository::getStatusVariableData(const std::vector<B1SECS2DataSVID>& svIDs)
{
    std::list<B1SECS2DataSV> result;
    for (const auto& svID : svIDs) {
        if (auto sv = _variableRepository->findStatusVariable(svID)) {
            result.push_back(sv->getValue());
        }
        else {
            result.push_back(SPB1SECS2Data(new B1SECS2DataASCII()));
        }
    }
    return result;
}
