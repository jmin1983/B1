//
// B1GEMDataRepository.h
//
// Library: B1GEM
// Package: B1GEM
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1GEM_DATA_REPOSITORY_H
#define _B1GEM_DATA_REPOSITORY_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2DataTypes.h>
#include "B1GEMDataValueCondition.h"
#include "B1GEMEventRepository.h"
#include "B1GEMReportRepository.h"

namespace BnD {
    class B1GEMAlarm;
    class B1GEMAlarmRepository;
    class B1GEMStatusVariable;
    class B1GEMVariableRepository;
    class B1GEMDataRepository {
    public:
        B1GEMDataRepository();
        virtual ~B1GEMDataRepository();
    protected:
        std::shared_ptr<B1GEMAlarmRepository> _alarmRepository;
        std::shared_ptr<B1GEMEventRepository> _eventRepository;
        std::shared_ptr<B1GEMVariableRepository> _variableRepository;
        std::shared_ptr<B1GEMReportRepository> _reportRepository;
    protected:
        virtual B1GEMAlarmRepository* createAlarmRepository(const B1SECS2DataManager* secs2DataManager) = 0;        //  return NULL if init failed.
        virtual B1GEMEventRepository* createEventRepository(const B1SECS2DataManager* secs2DataManager) = 0;        //  return NULL if init failed.
        virtual B1GEMVariableRepository* createVariableRepository(const B1SECS2DataManager* secs2DataManager) = 0;  //  return NULL if init failed.
    public:
        bool initialize(const B1SECS2DataManager* secs2DataManager);
        void finalize();

        std::shared_ptr<B1GEMAlarm> findAlarm(const B1SECS2DataALID& alID, bool isSetAlarm) const;

        void deleteAllReports();
        B1GEMEventRepository::RESULT changeEquipmentConstants(const std::map<B1SECS2DataECID, B1SECS2DataECV>& ecs);
        B1GEMEventRepository::RESULT linkReport(const std::map<B1SECS2DataCEID, std::vector<B1SECS2DataRPTID> >& linkEventData);
        bool getAvailableCEIDs(const std::vector<B1SECS2DataCEID>& ceIDs, std::vector<B1SECS2DataCEID>* result) const;
        SPGEMEvent findEvent(const B1SECS2DataCEID& ceID) const;

        void findEquipmentConstants(const std::vector<B1SECS2DataECID>& ecIDs,
                                    std::vector<B1SECS2DataECID>* resultECIDs, std::vector<B1SECS2DataECNAME>* ecNames, std::vector<B1SECS2DataECMIN>* ecMins,
                                    std::vector<B1SECS2DataECMAX>* ecMaxes, std::vector<B1SECS2DataECDEF>* ecDefs, std::vector<B1SECS2DataUNITS>* units);
        std::shared_ptr<B1GEMStatusVariable> findStatusVariable(const B1SECS2DataSVID& svID) const;
        std::map<B1SECS2DataSVID, std::pair<B1SECS2DataSVNAME, B1SECS2DataUNITS> > findStatusVariables(const std::vector<B1SECS2DataSVID>& svIDs) const;

        B1GEMReportRepository::RESULT updateReport(const std::map<B1SECS2DataRPTID, std::vector<B1SECS2DataVID> >& reportData);
        SPGEMReport findReport(const B1SECS2DataRPTID& rptID) const;
        std::vector<SPGEMReport> findReports(const std::vector<std::shared_ptr<B1SECS2DataRPTID> >& rptIDs) const;

        bool getEventReportData(const B1SECS2DataCEID& ceID, const B1GEMDataValueCondition& gemCondition,
                                std::map<B1SECS2DataRPTID, std::list<B1SECS2DataV> >* result);    //  return false if no need to send event
        std::list<B1SECS2DataECV> getEquipmentConstantValue(const std::vector<B1SECS2DataECID>& ecIDs);
        std::list<B1SECS2DataSV> getStatusVariableData(const std::vector<B1SECS2DataSVID>& svIDs);
    };
}   //  !BnD

#endif  // !_B1GEM_DATA_REPOSITORY_H
