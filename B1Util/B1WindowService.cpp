//
// B1WindowService.cpp
//
// Library: B1Util
// Package: B1Util
// Module:  B1Util
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Util.h"
#include "B1WindowService.h"
#include "B1WindowsUtil.h"

#include <B1Base/B1MainServiceInterface.h>
#include <B1Base/B1SystemUtil.h>
#include <B1Base/B1Thread.h>

#if defined(_WIN32)

using namespace BnD;

B1WindowService::B1WindowService(B1String&& serviceName)
    : _serviceName(std::move(serviceName))
    , _serviceHandle(0)
    , _mainService(0)
    , _currentState(0)
{
}

B1WindowService::~B1WindowService()
{
}

void B1WindowService::setServiceStatus(DWORD state, bool accept)
{
    const DWORD controlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PRESHUTDOWN;
    static DWORD checkPoint = 0;
    SERVICE_STATUS serviceStatus;
    serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    serviceStatus.dwCurrentState = state;
    serviceStatus.dwControlsAccepted = accept ? controlsAccepted : 0;
    serviceStatus.dwWin32ExitCode = 0;
    serviceStatus.dwServiceSpecificExitCode = 0;
    serviceStatus.dwCheckPoint = checkPoint++;
    serviceStatus.dwWaitHint = SERVICE_STOP_PENDING == state ? 60 * 1000 : 0;

    _currentState = state;
    ::SetServiceStatus(_serviceHandle, &serviceStatus);
}

void B1WindowService::serviceHandler(DWORD state)
{
    if (state == B1WindowService::get()->_currentState) {
        return;
    }

    switch (state) {
        case SERVICE_CONTROL_PAUSE:
            break;
        case SERVICE_CONTROL_CONTINUE:
            B1WindowService::get()->setServiceStatus(SERVICE_RUNNING);
            break;
        case SERVICE_CONTROL_SHUTDOWN:  //  for windows XP?
        case SERVICE_CONTROL_STOP:
            B1WindowService::get()->setServiceStatus(SERVICE_STOP_PENDING, false);
            if (B1WindowService::get()->_mainService) {
                B1WindowService::get()->_mainService->stop();
            }
            break;
        case SERVICE_CONTROL_PRESHUTDOWN:
            B1WindowService::get()->setServiceStatus(SERVICE_STOP_PENDING, false);
            if (B1WindowService::get()->_mainService) {
                B1WindowService::get()->_mainService->stop();
                for (int32 i = 0; i < 100; ++i) {
                    if (B1WindowService::get()->_mainService->isStopped()) {
                        break;
                    }
                    B1Thread::sleep(100);
                }
            }
            break;
        case SERVICE_CONTROL_INTERROGATE:
        default:
            B1WindowService::get()->setServiceStatus(B1WindowService::get()->_currentState);
            break;
    }
}

void B1WindowService::serviceMain(DWORD dwNumServicesArgs, LPSTR* lpServiceArgVectors)
{
    B1WindowService::get()->_serviceHandle = RegisterServiceCtrlHandlerA(B1WindowService::get()->_serviceName.cString(), (LPHANDLER_FUNCTION)serviceHandler);
    if (0 == B1WindowService::get()->_serviceHandle) {
        //DWORD lastError = GetLastError();
        return;
    }
    //B1WindowService::get()->setServiceStatus(SERVICE_START_PENDING);
    B1WindowService::get()->setServiceStatus(SERVICE_RUNNING);
    if (B1WindowService::get()->_mainService) {
        B1WindowService::get()->_mainService->start();
        B1WindowService::get()->_mainService->wait();
    }
    B1WindowService::get()->setServiceStatus(SERVICE_STOPPED);
}

bool B1WindowService::installService(const B1String& description)
{
    return B1WindowsUtil::createService(_serviceName, B1SystemUtil::getModuleFileName(), true, description) == ERROR_SUCCESS;
}

bool B1WindowService::uninstallService()
{
    return B1WindowsUtil::deleteService(_serviceName) == ERROR_SUCCESS;
}

bool B1WindowService::startService()
{
    return B1WindowsUtil::startService(_serviceName) == ERROR_SUCCESS;
}

bool B1WindowService::stopService()
{
    return B1WindowsUtil::stopService(_serviceName) == ERROR_SUCCESS;
}

bool B1WindowService::startServiceDispatch(B1MainServiceInterface* mainService)
{
    if (_serviceHandle != 0) {
        return false;
    }
    _mainService = mainService;
    char productName[1024] = { 0 };
    sprintf_s(productName, 1024, "%s", _serviceName.cString());
    SERVICE_TABLE_ENTRYA ste[] = { {productName, (LPSERVICE_MAIN_FUNCTIONA)serviceMain}, {NULL, NULL} };
    return StartServiceCtrlDispatcherA(ste) ? true : false;
}

#endif  //  _WIN32
