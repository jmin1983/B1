//
// B1WindowService.h
//
// Library: B1Util
// Package: B1Util
// Module:  B1Util
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1UTIL_WINDOWS_SERVICE_H
#define _B1UTIL_WINDOWS_SERVICE_H

#if defined(_WIN32)

#pragma once

#include <Windows.h>
#include <B1Base/B1Singleton.h>

namespace BnD {
    class B1MainServiceInterface;
    class B1WindowService : public B1Singleton<B1WindowService> {
    protected:
        B1WindowService(B1String&& serviceName);
    public:
        virtual ~B1WindowService();
    protected:
        const B1String _serviceName;
        SERVICE_STATUS_HANDLE _serviceHandle;
        B1MainServiceInterface* _mainService;
        DWORD _currentState;
    protected:
        static void serviceHandler(DWORD state);
        static void serviceMain(DWORD dwNumServicesArgs, LPSTR* lpServiceArgVectors);
        void setServiceStatus(DWORD state, bool accept = true);
    public:
        const B1String& serviceName() const { return _serviceName; }
        bool installService(const B1String& description);
        bool uninstallService();
        bool startService();
        bool stopService();
        bool startServiceDispatch(B1MainServiceInterface* mainService);
    };
}

#endif  //  _WIN32

#endif //   _B1UTIL_WINDOWS_SERVICE_H
