//
// B1WindowsUtil.h
//
// Library: B1Util
// Package: B1Util
// Module:  B1Util
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1UTIL_WINDOWS_UTIL_H
#define _B1UTIL_WINDOWS_UTIL_H

#if defined(_WIN32)

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <Windows.h>
#include <atlstr.h>
#include <list>

namespace BnD {
    class B1WindowsUtil {
    public:
        enum PROCESS_WAIT_METHOD {
            SINGLE_OBJECT = 0,
            INPUT_IDLE,
            NO_WAIT,
        };
        enum WINDOWS_VERSION {
            VERSION_UNKNOWN = -1,
            VERSION_XP,
            VERSION_VISTA,
            VERSION_7,
            VERSION_8,
            VERSION_8_1,
            VERSION_10
        };
    public:
        static DWORD createProcess(const B1String& process, const B1String& workDirectory = B1String(), WORD showWindow = SW_SHOW, PROCESS_WAIT_METHOD waitMethod = SINGLE_OBJECT, DWORD timeout = INFINITE, LPDWORD exitCode = NULL);
        static DWORD createProcess(const B1String& process, DWORD session, bool system = false, const B1String& workDirectory = B1String(), WORD showWindow = SW_SHOW, PROCESS_WAIT_METHOD waitMethod = SINGLE_OBJECT, LPDWORD exitCode = NULL);
        static DWORD createProcessRunas(const B1String& process, const B1String& workDirectory, HWND hwnd, int32 show, const B1String& parameter = B1String(), bool wait = false);
        static DWORD createMediumProcess(const B1String& process, const B1String& workDirectory = B1String(), WORD showWindow = SW_SHOW, PROCESS_WAIT_METHOD waitMethod = SINGLE_OBJECT);
        static DWORD createProcessToAllSession(const B1String& process, const B1String& workDirectory, WORD showWindow = SW_SHOW, PROCESS_WAIT_METHOD waitMethod = SINGLE_OBJECT, bool system = false);
        static DWORD terminateProcess(const B1String& process, DWORD milliSeconds);
        static DWORD getOSVersion(DWORD* major, DWORD* minor = NULL, WORD* pack = NULL);
        static DWORD getSystemDirectory(B1String* directory);
        static DWORD createMiniDump(const B1String& processName);

        static DWORD enableFirewall(const B1String& programFileFullPath, const B1String& programName, bool enable, DWORD wait = 5000);
        static DWORD copySubDirectoryAndFiles(const B1String& srcDirFullPath, const B1String& dstDirFullPath);
        static DWORD getSubDirectories(const B1String& srcDirFullPath, std::list<B1String>* directories);
        static int32 isDirectory(const WIN32_FIND_DATAA& fileData); //  return: if directory 1, if file 0, if . or .. -1.

        static bool isProcessRunning(const B1String& processName, DWORD* pid = NULL);  //  매우 부하가 크다. 너무 빨리 반복적 사용 금지.
        static bool isProcessRunningInSession(DWORD session, const B1String& processName, DWORD* pid = NULL);
        static bool isSystem64Bit();
        static bool isWindowVistaSameOrLater();
        static bool isWindow8SameOrLater();
        static bool isSameOSVersion(WINDOWS_VERSION version);

        static DWORD createService(const B1String& serviceName, const B1String& serviceFile, bool autoRecover, const B1String& desc, const B1String& dependency = B1String(), bool delayStart = false);
        static DWORD modifyService(const B1String& serviceName, const B1String& desc, const B1String& dependencies = B1String());
        static DWORD deleteService(const B1String& serviceName);
        static DWORD startService(const B1String& serviceName);
        static DWORD stopService(const B1String& serviceName);
        static DWORD disableWindowsService(const B1String& serviceName);
        static bool isServiceInstalled(const B1String& serviceName);

        static bool setProcessHighPriority();
    };
}

#endif  //  _WIN32

#endif //   _B1UTIL_WINDOWS_UTIL_H
