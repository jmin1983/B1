//
// B1MainService.h
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1BASE_MAIN_SERVICE_H
#define _B1BASE_MAIN_SERVICE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Base/B1Condition.h>
#include <B1Base/B1String.h>

#include <map>
 
namespace BnD {
    class B1MainService {
    public:
        B1MainService(int32 serviceID, int32 version, B1String&& buildDate, B1String&& serviceName, B1String&& systemName);
        virtual ~B1MainService();
    private:
        enum CONSTS {
            CONSTS_MAIN_THREAD_PROCESS_INTERVAL_DEFAULT = 100,
            CONSTS_MAIN_THREAD_PROCESS_INTERVAL_MIN = 20,
        };
        enum MAIN_SERVICE_STATUS {
            MAIN_SERVICE_STATUS_WORKING = 0,
            MAIN_SERVICE_STATUS_STOPPING,
            MAIN_SERVICE_STATUS_STOPPED,
        };
        const int32 _serviceID;
        const int32 _version;
        const B1String _buildDate;
        const B1String _mainServiceName;
        const B1String _systemServiceName;
        MAIN_SERVICE_STATUS _mainServiceStatus;
        B1Condition _mainCondition;
    protected:
        virtual bool implStart() { return true;  }
        virtual bool implWorking() { return true; } //  return false to stop B1MainService
        virtual void implStop() {}
        virtual std::map<B1String, B1String> makeVersionInfoMap() const;
    public:
        bool start();
        void wait();
        void stop();
        bool isWorking() const { return MAIN_SERVICE_STATUS_WORKING == _mainServiceStatus; }
        bool isStopped() const { return MAIN_SERVICE_STATUS_STOPPED == _mainServiceStatus; }
        B1String softwareRev() const;
        B1String softwareDate() const;
        B1String toString() const;
        int32 serviceID() const { return _serviceID; }
        const B1String& mainServiceName() const { return _mainServiceName; }
    };
}   //  !BnD

#endif  // !_B1BASE_MAIN_SERVICE_H
