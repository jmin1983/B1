//
// B1WindowsUtil.cpp
//
// Library: B1Util
// Package: B1Util
// Module:  B1Util
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Util.h"
#include "B1WindowsUtil.h"

#include <B1Base/B1StringUtil.h>
#include <B1Base/B1SystemUtil.h>
#include <B1Base/B1Time.h>

#if defined(_WIN32)

#include <WinSvc.h>
#include <sddl.h>
#if defined(UNICODE)
#undef UNICODE
#include <tlhelp32.h>
#define UNICODE
#else
#include <tlhelp32.h>
#endif
#include <ShellAPI.h>
#include <ShlObj.h>
#include <WtsApi32.h>
#include <Userenv.h>
#include <Dbghelp.h>
#include <atltime.h>
#include <stdio.h>
#include <share.h>
#include <Ntsecapi.h>

#pragma comment (lib, "shell32.lib")
#pragma comment (lib, "WtsApi32.lib")
#pragma comment (lib, "Userenv.lib")
#pragma comment (lib, "Dbghelp.lib")
#pragma warning (disable: 4311)     //  no 64bit

using namespace BnD;

DWORD B1WindowsUtil::createProcess(const B1String& process, const B1String& workDirectory, WORD showWindow, PROCESS_WAIT_METHOD waitMethod, DWORD timeout, LPDWORD exitCode)
{
    DWORD returnValue = ERROR_SUCCESS;
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb			= sizeof(si);
    si.dwFlags		= STARTF_USESHOWWINDOW;
    si.wShowWindow	= showWindow;
    if (::CreateProcessA(NULL, const_cast<char*>(process.cString()), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, workDirectory.cString(), &si, &pi)) {
        switch (waitMethod) {
            case SINGLE_OBJECT: {
                WaitForSingleObject(pi.hProcess, timeout);
                if (exitCode) {
                    GetExitCodeProcess(pi.hProcess, exitCode);
                }
                break;
            }
            case INPUT_IDLE:
                WaitForInputIdle(pi.hProcess, timeout);
                break;
            case NO_WAIT:
                break;
        }
    }
    else {
        returnValue = GetLastError();
    }
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return returnValue;
}

DWORD B1WindowsUtil::createProcess(const B1String& process, DWORD session, bool system, const B1String& workDirectory, WORD showWindow, PROCESS_WAIT_METHOD waitMethod, LPDWORD exitCode)
{
    if (isWindowVistaSameOrLater()) {
        DWORD currentSession = 0xFFFFFFFF;
        if (::ProcessIdToSessionId(::GetCurrentProcessId(), &currentSession)) {
            if (currentSession == session) {
                return createProcess(process, workDirectory, showWindow, waitMethod, INFINITE, exitCode);
            }
        }
    }

    DWORD pid = 0;
    if (system &&
        isWindowVistaSameOrLater()) {
        if (!isProcessRunningInSession(session, "winlogon.exe", &pid)) {
            return ERROR_FILE_NOT_FOUND;
        }
    }
    else {
        if (!isProcessRunningInSession(session, "explorer.exe", &pid)) {
            if (isWindow8SameOrLater()) {
                if (!isProcessRunningInSession(session, "eshell.exe", &pid)) {
                    return ERROR_FILE_NOT_FOUND;
                }
            }
            else {
                return ERROR_FILE_NOT_FOUND;
            }
        }
    }

    HANDLE hProcess = ::OpenProcess(MAXIMUM_ALLOWED, FALSE, pid);
    if (hProcess == NULL) {
        return GetLastError();
    }
    HANDLE hToken = NULL;
    if (!::OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY|TOKEN_DUPLICATE|TOKEN_ASSIGN_PRIMARY|TOKEN_ADJUST_SESSIONID|TOKEN_READ|TOKEN_WRITE, &hToken)) {
        CloseHandle(hProcess);
        return GetLastError();
    }
    LUID luid;
    if (!::LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid)) {
        CloseHandle(hProcess);
        CloseHandle(hToken);
        return GetLastError();
    }
    HANDLE hTokenDup = NULL;
    TOKEN_PRIVILEGES tp;
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    if (!DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL, SecurityIdentification, TokenPrimary, &hTokenDup)) {
        CloseHandle(hProcess);
        CloseHandle(hToken);
        return GetLastError();
    }
    ::SetTokenInformation(hTokenDup, TokenSessionId, (LPVOID)&session, sizeof(DWORD));
    if (!::AdjustTokenPrivileges(hTokenDup, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL,NULL)) {
        CloseHandle(hTokenDup);
        CloseHandle(hToken);
        CloseHandle(hProcess);
        return GetLastError();
    }
    //  UAC 켰을 때 아래 코드에서 걸린다. 실제 프로세스 실행은 되므로 주석처리.
    //if (GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
    //    CloseHandle(hTokenDup);
    //    CloseHandle(hToken);
    //    CloseHandle(hProcess);
    //    return GetLastError();
    //}
    STARTUPINFOA si;
    ZeroMemory(&si, sizeof(STARTUPINFOA));
    si.cb = sizeof(STARTUPINFOA);
    si.lpDesktop = (LPSTR)"winsta0\\default";
    si.wShowWindow = showWindow;
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    DWORD dwCreationFlags = NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE;
    LPVOID pEnv = NULL;
    if (::CreateEnvironmentBlock(&pEnv, hTokenDup, TRUE)) {
        dwCreationFlags |= CREATE_UNICODE_ENVIRONMENT;
    } else {
        pEnv = NULL;
    }

    LPSTR lpszPath = _strdup(process.cString());
    if (::CreateProcessAsUserA(hTokenDup, NULL, lpszPath, NULL, NULL, FALSE, dwCreationFlags, pEnv, workDirectory.cString(), &si, &pi)) {
        switch (waitMethod) {
            case SINGLE_OBJECT: {
                WaitForSingleObject(pi.hProcess, INFINITE);
                if (exitCode) {
                    GetExitCodeProcess(pi.hProcess, exitCode);
                }
                break;
            }
            case INPUT_IDLE:
                WaitForInputIdle(pi.hProcess, INFINITE);
                break;
            case NO_WAIT:
                break;
        }
    }
    else {
        CloseHandle(hTokenDup);
        CloseHandle(hToken);
        CloseHandle(hProcess);
        free(lpszPath);
        return GetLastError();
    }
    free(lpszPath);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    CloseHandle(hTokenDup);
    CloseHandle(hToken);
    CloseHandle(hProcess);

    return ERROR_SUCCESS;
}

DWORD B1WindowsUtil::createProcessRunas(const B1String& process, const B1String& workDirectory, HWND hwnd, int32 show, const B1String& parameter, bool wait)
{
    SHELLEXECUTEINFOA sei;
    ZeroMemory(&sei, sizeof(sei));
    sei.cbSize = sizeof(sei);

    sei.fMask = SEE_MASK_FLAG_DDEWAIT | SEE_MASK_FLAG_NO_UI;
    sei.hwnd = hwnd;
    sei.lpVerb = "runas";
    sei.lpFile = process.cString();
    sei.lpDirectory = workDirectory.cString();
    sei.lpParameters = parameter.cString();
    sei.nShow = show;

    DWORD result = ERROR_SUCCESS;
    if (ShellExecuteExA(&sei) != TRUE) {
        result = GetLastError();
    }
    if (wait) {
        ::WaitForSingleObject(sei.hProcess, INFINITE);
    }
    return result;
}

DWORD B1WindowsUtil::createMediumProcess(const B1String& process, const B1String& workDirectory, WORD showWindow, PROCESS_WAIT_METHOD waitMethod)
{
    DWORD returnValue = ERROR_SUCCESS;
    HANDLE hToken;
    HANDLE hNewToken;

    // LowIL : S-1-16-4096
    // MediumIL : S-1-16-8192
    // HighIL : S-1-16-12288
    // SystemIL : S-1-16-16384
    const char wszIntegritySid[20] = "S-1-16-8192";

    PSID pIntegritySid = NULL;

    TOKEN_MANDATORY_LABEL TIL = {0};
    PROCESS_INFORMATION ProcInfo = {0};
    STARTUPINFO StartupInfo = {0};

    if (OpenProcessToken(GetCurrentProcess(), MAXIMUM_ALLOWED, &hToken)) {
        if (DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL, SecurityImpersonation, TokenPrimary, &hNewToken)) {
            if (ConvertStringSidToSidA(wszIntegritySid, &pIntegritySid)) {
                TIL.Label.Attributes = SE_GROUP_INTEGRITY;
                TIL.Label.Sid = pIntegritySid;

                // Set the process integrity level
                if (SetTokenInformation(hNewToken, TokenIntegrityLevel, &TIL, sizeof(TOKEN_MANDATORY_LABEL) + GetLengthSid(pIntegritySid))) {
                    // Create the new process at Low/High integrity
                    STARTUPINFOA si;
                    PROCESS_INFORMATION pi;
                    ZeroMemory(&si, sizeof(STARTUPINFO));
                    si.cb			= sizeof(si);
                    si.dwFlags		= STARTF_USESHOWWINDOW;
                    si.wShowWindow	= showWindow;

                    if (::CreateProcessAsUserA(hNewToken, NULL, const_cast<char*>(process.cString()), NULL, NULL, FALSE, 0, NULL, workDirectory.cString(), &si, &pi)) {
                        switch (waitMethod) {
                            case SINGLE_OBJECT:
                                WaitForSingleObject(pi.hProcess, INFINITE);
                                break;
                            case INPUT_IDLE:
                                WaitForInputIdle(pi.hProcess, INFINITE);
                                break;
                            case NO_WAIT:
                                break;
                        }
                    }
                    else {
                        returnValue = GetLastError();
                    }
                }
                LocalFree(pIntegritySid);
            }
            CloseHandle(hNewToken);
        }
        CloseHandle(hToken);
    }
    return returnValue;
}

DWORD B1WindowsUtil::createProcessToAllSession(const B1String& process, const B1String& workDirectory, WORD showWindow, PROCESS_WAIT_METHOD waitMethod, bool system)
{
    WTS_SESSION_INFOA* info;
    DWORD infoCount;
    if (!::WTSEnumerateSessionsA(WTS_CURRENT_SERVER_HANDLE, NULL, 1, &info, &infoCount)) {
        return GetLastError();
    }

    for (DWORD i = 0; i < infoCount; ++i) {
        if (info[i].State != WTSActive) {
            continue;
        }
        DWORD result = createProcess(process, info[i].SessionId, system, workDirectory, showWindow, waitMethod);
        if (result != ERROR_SUCCESS) {
            return result;
        }
    }
    WTSFreeMemory(info);
    return ERROR_SUCCESS;
}

DWORD B1WindowsUtil::terminateProcess(const B1String& process, DWORD milliSeconds)
{
    DWORD pid = 0;
    if (B1WindowsUtil::isProcessRunning(process, &pid) == TRUE) {
        HANDLE hProcess = OpenProcess(MAXIMUM_ALLOWED, FALSE, pid);
        if (hProcess == NULL) {
            return GetLastError();
        }
        DWORD exitCode;
        if (GetExitCodeProcess(hProcess, &exitCode) == FALSE) {
            CloseHandle(hProcess);
            return GetLastError();
        }
        if (TerminateProcess(hProcess, exitCode) == FALSE) {
            CloseHandle(hProcess);
            return GetLastError();
        }
        DWORD wait = WaitForSingleObject(hProcess, milliSeconds);
        if (wait == WAIT_FAILED) {
            CloseHandle(hProcess);
            return GetLastError();
        }
        if (wait == WAIT_TIMEOUT) {
            CloseHandle(hProcess);
            return WAIT_TIMEOUT;
        }
        CloseHandle(hProcess);
    }
    return ERROR_SUCCESS;
}

DWORD B1WindowsUtil::getOSVersion(DWORD* major, DWORD* minor, WORD* pack)
{
    struct WinVersionInfo { DWORD mj; DWORD mi; WORD pa; } vi[] = {
        {10, 0, 0 },//win10, server2016

        { 6, 3, 0 },//win8.1,server2012 r2
        { 6, 2, 0 },//win8,server2012

        { 6, 1, 1 },//win7,win2008r2 sp1
        { 6, 1, 0 },//win7,win2008r2

        { 5, 1, 3 },//winxp sp3
        { 5, 1, 2 },//winxp sp2
        { 5, 1, 1 },//winxp sp1
        { 5, 1, 0 },//winxp

        { 6, 0, 2 },//WinVista,server2008 SP2
        { 6, 0, 1 },//WinVista,Server2008 Sp1
        { 6, 0, 0 },//WinVista,Server2008

        { 5, 2, 2 },//Windows Server 2003 Sp2
        { 5, 2, 1 },//Windows Server 2003 Sp1
        { 5, 2, 0 },//Windows Server 2003

        { 5, 1, 4 }, //Windows Server 2000 Sp4
        { 5, 1, 3 }, //Windows Server 2000 Sp3
        { 5, 1, 2 }, //Windows Server 2000 Sp2
        { 5, 1, 2 }, //Windows Server 2000 Sp1
        { 5, 1, 0 }, //Windows Server 2000
    };

    OSVERSIONINFOEX osVersionInfo;

#define EQUALS_VERSION(type, ver, res)\
    ::ZeroMemory(&osVersionInfo, sizeof(OSVERSIONINFOEX));\
    osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);\
    switch (type) {\
        case VER_MAJORVERSION: osVersionInfo.dwMajorVersion = (DWORD)ver; break;\
        case VER_MINORVERSION: osVersionInfo.dwMinorVersion = (DWORD)ver; break;\
        case VER_SERVICEPACKMAJOR: osVersionInfo.wServicePackMajor = (WORD)ver; break;\
    }\
    ULONGLONG maskCondition = ::VerSetConditionMask(0, type, VER_EQUAL);\
    res = ::VerifyVersionInfo(&osVersionInfo, type, maskCondition) ? true : false;

    bool found = false;
    for (int i = 0; i < _countof(vi); ++i) {
        bool eqMajor = false;
        EQUALS_VERSION(VER_MAJORVERSION, vi[i].mj, eqMajor);
        if (eqMajor) {
            bool eqMinor = false;
            EQUALS_VERSION(VER_MINORVERSION, vi[i].mi, eqMinor);
            if (eqMinor) {
                bool eqPack = false;
                EQUALS_VERSION(VER_SERVICEPACKMAJOR, vi[i].pa, eqPack);
                if (eqPack) {
                    if (major) {
                        *major = vi[i].mj;
                    }
                    if (minor) {
                        *minor = vi[i].mi;
                    }
                    if (pack) {
                        *pack = vi[i].pa;
                    }
                    found = true;
                    break;
                }
            }
        }
    }
    return found ? ERROR_SUCCESS : ERROR_OLD_WIN_VERSION;
}

DWORD B1WindowsUtil::getSystemDirectory(B1String* directory)
{
    char buffer[MAX_PATH] = { 0 };
    if (::GetWindowsDirectoryA(buffer, MAX_PATH) == 0) {
        return GetLastError();
    }
    for (size_t i = 0; i < strlen(buffer); ++i) {
        if (buffer[i] == '\\') {
            buffer[i + 1] = 0;
            *directory = buffer;
            return ERROR_SUCCESS;
        }
    }
    return ERROR_PATH_NOT_FOUND;
}

DWORD B1WindowsUtil::createMiniDump(const B1String& processName)
{
    DWORD pid = 0;
    if (!isProcessRunning(processName, &pid)) {
        return ERROR_BAD_PATHNAME;
    }
    HANDLE hProcess = OpenProcess(MAXIMUM_ALLOWED, FALSE, pid);
    if (!hProcess) {
        return GetLastError();
    }

    B1String current = B1SystemUtil::getCurrentDirectory();
    CTime time = CTime::GetCurrentTime();
    char filePath[MAX_PATH] = { 0 };
    sprintf_s(filePath, MAX_PATH, "%s\\%s.forced.%d%02d%02d%02d%02d%02d.dmp", current.cString(), processName.cString(),
                time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
    HANDLE hFile = CreateFileA(filePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (!hFile) {
        CloseHandle(hProcess);
        return GetLastError();
    }
    if (!MiniDumpWriteDump(hProcess, pid, hFile, MiniDumpNormal, NULL, NULL, NULL)) {
        CloseHandle(hFile);
        CloseHandle(hProcess);
        return GetLastError();
    }
    CloseHandle(hFile);
    CloseHandle(hProcess);
    return ERROR_SUCCESS;
}

DWORD B1WindowsUtil::enableFirewall(const B1String& programFileFullPath, const B1String& programName, bool enable, DWORD wait /*= 5000*/)
{
    char param[1024] = { 0 };
    if (enable) {
        if (isWindowVistaSameOrLater()) {
            sprintf_s(param, 1024, "advfirewall firewall add rule name=\"%s\" dir=in action=allow program=\"%s\" enable=yes", programName.cString(), programFileFullPath.cString());
        }
        else {
            sprintf_s(param, 1024, "firewall add allowedprogram \"%s\" \"%s\" ENABLE", programFileFullPath.cString(), programName.cString());
        }
    }
    else {
        if (isWindowVistaSameOrLater()) {
            sprintf_s(param, 1024, "advfirewall firewall delete rule name=\"%s\"", programName.cString());
        }
        else {
            sprintf_s(param, 1024, "firewall delete allowedprogram \"%s\"", programFileFullPath.cString());
        }
    }
    SHELLEXECUTEINFOA sei = { 0 };
    sei.cbSize = sizeof(sei);
    sei.lpVerb = "open";
    sei.lpFile = "netsh.exe";
    sei.lpParameters = param;
    sei.nShow = SW_HIDE;
    sei.fMask |= SEE_MASK_NOCLOSEPROCESS;
    if (ShellExecuteExA(&sei) != TRUE) {
        return GetLastError();
    }
    SetPriorityClass(sei.hProcess, HIGH_PRIORITY_CLASS);
    WaitForSingleObject(sei.hProcess, wait);
    CloseHandle(sei.hProcess);
    return ERROR_SUCCESS;
}

DWORD B1WindowsUtil::copySubDirectoryAndFiles(const B1String& srcDirFullPath, const B1String& dstDirFullPath)
{
    if (::GetFileAttributesA(srcDirFullPath.cString()) == INVALID_FILE_ATTRIBUTES) {
        return ERROR_SUCCESS;   //  success if not exist.
    }
    if (::GetFileAttributesA(dstDirFullPath.cString()) == INVALID_FILE_ATTRIBUTES) {
        B1SystemUtil::createDirectory(dstDirFullPath);
    }

    char all[MAX_PATH] = { 0 };
    sprintf_s(all, MAX_PATH, "%s\\*", srcDirFullPath.cString());

#define COPY_DIRECTORY_OR_FILE\
    char src[MAX_PATH] = { 0 };\
    sprintf_s(src, MAX_PATH, "%s\\%s", srcDirFullPath.cString(), findFileData.cFileName);\
    char dst[MAX_PATH] = { 0 };\
    sprintf_s(dst, MAX_PATH, "%s\\%s", dstDirFullPath.cString(), findFileData.cFileName);\
    switch (isDirectory(findFileData)) {\
        case 1: { copySubDirectoryAndFiles(src, dst); break; }\
        case 0: { ::CopyFileA(src, dst, FALSE); break; }\
        default: break;\
    }

    WIN32_FIND_DATAA findFileData;
    HANDLE findHandle = INVALID_HANDLE_VALUE;
    findHandle = FindFirstFileA(all, &findFileData);
    if (findHandle == INVALID_HANDLE_VALUE) {
        return GetLastError();
    }
    else {
        COPY_DIRECTORY_OR_FILE;
        while (FindNextFileA(findHandle, &findFileData) != 0) {
            COPY_DIRECTORY_OR_FILE;
        }
        DWORD error = GetLastError();
        FindClose(findHandle);
        if (error != ERROR_NO_MORE_FILES) {
            return error;
        }
    }
    return ERROR_SUCCESS;
}

DWORD B1WindowsUtil::getSubDirectories(const B1String& srcDirFullPath, std::list<B1String>* directories)
{
    B1String path = srcDirFullPath.copy();
    B1StringUtil::removeLastPathSeparator(&path);
    WIN32_FIND_DATAA fd;
    HANDLE handle = ::FindFirstFileA((path + "\\*").cString(), &fd);
    if (INVALID_HANDLE_VALUE == handle) {
        return GetLastError();
    }
    directories->clear();
    do {
        if (isDirectory(fd) == 1) {
            directories->push_back(B1String(fd.cFileName));
        }
    } while (::FindNextFileA(handle, &fd));
    ::FindClose(handle);
    return ERROR_SUCCESS;
}

int32 B1WindowsUtil::isDirectory(const WIN32_FIND_DATAA& fileData)
{
    if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        if (!strcmp(fileData.cFileName, ".") ||
            !strcmp(fileData.cFileName, "..")) {
            return -1;
        }
        return 1;
    }
    return 0;
}

bool B1WindowsUtil::isProcessRunning(const B1String& processName, DWORD* pid)
{
    HANDLE processSnap;
    PROCESSENTRY32 pe32;
    // Take a snapshot of all processes in the system.
    processSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    if (processSnap == INVALID_HANDLE_VALUE) {
        return false;
    }

    // Set the size of the structure before using it.
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Retrieve information about the first process,
    // and exit if unsuccessful
    if (!Process32First(processSnap, &pe32)) {
        CloseHandle(processSnap);     // Must clean up the snapshot object!
        return false;
    }

    // Now walk the snapshot of processes, and
    // display information about each process in turn
    do {
        if (strcmp(processName.cString(), pe32.szExeFile)) {
            continue;
        }
        else {
            CloseHandle(processSnap);
            if (pid) {
                *pid = pe32.th32ProcessID;
            }
            return true;
        }
    } while(Process32Next(processSnap, &pe32));

    CloseHandle(processSnap);
    return false;
}

bool B1WindowsUtil::isProcessRunningInSession(DWORD session, const B1String& processName, DWORD* pid)
{
    HANDLE processSnap;
    PROCESSENTRY32 pe32;

    // Take a snapshot of all processes in the system.
    processSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (processSnap == INVALID_HANDLE_VALUE) {
        return FALSE;
    }

    // Set the size of the structure before using it.
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Retrieve information about the first process,
    // and exit if unsuccessful
    if (!Process32First(processSnap, &pe32)) {
        CloseHandle(processSnap);     // Must clean up the snapshot object!
        return FALSE;
    }

    // Now walk the snapshot of processes, and
    // display information about each process in turn
    do {
        if (strcmp(processName.cString(), pe32.szExeFile)) {
            continue;
        }
        else {
            DWORD sessionID = 0;
            if (::ProcessIdToSessionId(pe32.th32ProcessID, &sessionID)) {
                if (session == sessionID) {
                    CloseHandle(processSnap);
                    if (pid) {
                        *pid = pe32.th32ProcessID;
                    }
                    return TRUE;
                }
            }
        }
    } while(Process32Next(processSnap, &pe32));

    CloseHandle(processSnap);
    return FALSE;
}

bool B1WindowsUtil::isSystem64Bit()
{
    SYSTEM_INFO info;
    ::GetNativeSystemInfo(&info);
    return (info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64);
}

bool B1WindowsUtil::isWindowVistaSameOrLater()
{
    bool result = false;
    DWORD major = 0;
    if (getOSVersion(&major, NULL) == ERROR_SUCCESS) {
        result = (major >= 6);
    }
    return result;
}

bool B1WindowsUtil::isWindow8SameOrLater()
{
    bool result = false;
    DWORD major = 0, minor = 0;
    if (getOSVersion(&major, &minor) == ERROR_SUCCESS) {
        result = major > 6 ||
                 (major == 6 && minor >= 2);
    }
    return result;
}

bool B1WindowsUtil::isSameOSVersion(WINDOWS_VERSION version)
{
#define CASE_CHECK_VERSION(version, vmajor, vminor) case version: if (major == vmajor && minor == vminor) return true;

    DWORD major, minor;
    WORD pack;

    if (getOSVersion(&major, &minor, &pack) != ERROR_SUCCESS) return false;

    switch (version) {
        CASE_CHECK_VERSION(VERSION_XP, 5, 1)
        CASE_CHECK_VERSION(VERSION_VISTA, 6, 0)
        CASE_CHECK_VERSION(VERSION_7, 6, 1)
        CASE_CHECK_VERSION(VERSION_8, 6, 2)
        CASE_CHECK_VERSION(VERSION_8_1, 6, 3)
        CASE_CHECK_VERSION(VERSION_10, 10, 0)
    }
    return false;
}

DWORD B1WindowsUtil::createService(const B1String& serviceName, const B1String& serviceFile, bool autoRecover, const B1String& desc, const B1String& dependency, bool delayStart)
{
    SC_HANDLE hManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hManager == NULL) {
        return GetLastError();  //  ERROR_ACCESS_DENIED, ERROR_DATABASE_DOES_NOT_EXIST, ERROR_INVALID_PARAMETER
    }
    SC_HANDLE hService = CreateServiceA(hManager, serviceName.cString(), serviceName.cString(),
                                      SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
                                      SERVICE_AUTO_START, SERVICE_ERROR_IGNORE,
                                      serviceFile.cString(), NULL, NULL, dependency.cString(), NULL, NULL);
    CloseServiceHandle(hManager);
    if (hService == NULL) {
        return GetLastError();  //  ERROR_SERVICE_MARKED_FOR_DELETE(1072), ERROR_SERVICE_EXISTS(1073)
    }

    if (desc.isEmpty() != true) {
        char temp[MAX_PATH] = { 0 };
        sprintf_s(temp, MAX_PATH, "%s", desc.cString());
        SERVICE_DESCRIPTIONA d = { 0 };
        d.lpDescription = temp;
        ::ChangeServiceConfig2A(hService, SERVICE_CONFIG_DESCRIPTION, &d);
    }
    if (delayStart &&
        isWindowVistaSameOrLater()) {
        SERVICE_DELAYED_AUTO_START_INFO info;
        info.fDelayedAutostart = TRUE;
        ::ChangeServiceConfig2A(hService, SERVICE_CONFIG_DELAYED_AUTO_START_INFO, &info);
    }
    if (autoRecover) {
        SERVICE_FAILURE_ACTIONSA serviceFailureActions = {0};
        SC_ACTION serviceAction[3];
        serviceAction[0].Delay = 2000;
        serviceAction[0].Type = SC_ACTION_RESTART;
        serviceAction[1].Delay = 2000;
        serviceAction[1].Type = SC_ACTION_RESTART;
        serviceAction[2].Delay = 2000;
        serviceAction[2].Type = SC_ACTION_RESTART;

        serviceFailureActions.dwResetPeriod = 600;
        serviceFailureActions.lpRebootMsg = (LPSTR)"";
        serviceFailureActions.lpCommand = (LPSTR)"";
        serviceFailureActions.cActions = 3;
        serviceFailureActions.lpsaActions = (SC_ACTION*)serviceAction;

        ::ChangeServiceConfig2A(hService, SERVICE_CONFIG_FAILURE_ACTIONS, &serviceFailureActions);
    }

    CloseServiceHandle(hService);
    return ERROR_SUCCESS;
}

DWORD B1WindowsUtil::modifyService(const B1String& serviceName, const B1String& desc, const B1String& dependencies)
{
    SC_HANDLE hManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hManager == NULL) {
        return GetLastError();  //  ERROR_ACCESS_DENIED, ERROR_DATABASE_DOES_NOT_EXIST, ERROR_INVALID_PARAMETER
    }
    SC_HANDLE hService = OpenServiceA(hManager, serviceName.cString(), SERVICE_ALL_ACCESS);
    CloseServiceHandle(hManager);
    if (hService == NULL) {
        return GetLastError();  //  ERROR_SERVICE_MARKED_FOR_DELETE(1072), ERROR_SERVICE_EXISTS(1073)
    }
    if (desc.isEmpty() != true) {
        char temp[MAX_PATH] = { 0 };
        sprintf_s(temp, MAX_PATH, "%s", desc.cString());
        SERVICE_DESCRIPTIONA d = { 0 };
        d.lpDescription = temp;
        ::ChangeServiceConfig2A(hService, SERVICE_CONFIG_DESCRIPTION, &d);
    }
    if (dependencies.isEmpty() != true) {
        ChangeServiceConfigA(hService, SERVICE_NO_CHANGE, SERVICE_NO_CHANGE, SERVICE_NO_CHANGE, NULL, NULL, NULL, dependencies.cString(), NULL, NULL, NULL);
    }
    CloseServiceHandle(hService);
    return ERROR_SUCCESS;
}

DWORD B1WindowsUtil::deleteService(const B1String& serviceName)
{
    SC_HANDLE hManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hManager == NULL) {
        return GetLastError();  //  ERROR_ACCESS_DENIED, ERROR_DATABASE_DOES_NOT_EXIST, ERROR_INVALID_PARAMETER
    }
    SC_HANDLE hService = OpenServiceA(hManager, serviceName.cString(), SERVICE_ALL_ACCESS);
    CloseServiceHandle(hManager);
    if (hService == NULL) {
        return GetLastError();  //  ERROR_SERVICE_MARKED_FOR_DELETE(1072)
    }
    if (!DeleteService(hService)) {
        CloseServiceHandle(hService);
        return GetLastError();  //  ERROR_SERVICE_MARKED_FOR_DELETE(1072)
    }
    CloseServiceHandle(hService);
    return ERROR_SUCCESS;
}

DWORD B1WindowsUtil::startService(const B1String& serviceName)
{
    SC_HANDLE hManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hManager == NULL) {
        return GetLastError();  //  ERROR_ACCESS_DENIED, ERROR_DATABASE_DOES_NOT_EXIST, ERROR_INVALID_PARAMETER
    }

    SC_HANDLE hService = OpenServiceA(hManager, serviceName.cString(), SERVICE_ALL_ACCESS);
    CloseServiceHandle(hManager);
    if (hService == NULL) {
        return GetLastError();  //  ERROR_ACCESS_DENIED, ERROR_INVALID_NAME, ERROR_SERVICE_DOES_NOT_EXIST(1060)
    }
    if (!StartServiceA(hService, 0, NULL)) {
        CloseServiceHandle(hService);
        DWORD reason = GetLastError();
        if (reason == ERROR_SERVICE_ALREADY_RUNNING) {
            return ERROR_SUCCESS;
        }
        else {
            return reason;  //  ERROR_PATH_NOT_FOUND, ERROR_SERVICE_DATABASE_LOCKED, ERROR_SERVICE_DISABLED(1058)
        }
    }
    SERVICE_STATUS_PROCESS ssStatus;
    DWORD dwBytesNeeded;
    if (!QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO, (LPBYTE)&ssStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded)) {
        CloseServiceHandle(hService);
        return GetLastError();  //  ERROR_ACCESS_DENIED, ERROR_INSUFFICIENT_BUFFER
    }

    DWORD dwStartTickCount = GetTickCount();
    DWORD dwOldCheckPoint = ssStatus.dwCheckPoint;

    while (ssStatus.dwCurrentState == SERVICE_START_PENDING)
    {
        DWORD dwWaitTime = ssStatus.dwWaitHint / 10;  //  A good interval is one tenth the wait hint.
        if (dwWaitTime < 1000) { //  no less than 1 second.
            dwWaitTime = 1000;
        }
        else if (dwWaitTime > 10000) {  //   no more than 10 seconds.
            dwWaitTime = 10000;
        }
        Sleep(dwWaitTime);

        if (!QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO, (LPBYTE)&ssStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded)) {
            break;
        }

        if (ssStatus.dwCheckPoint > dwOldCheckPoint) {
            dwStartTickCount = GetTickCount();
            dwOldCheckPoint = ssStatus.dwCheckPoint;
        }
        else {
            if (GetTickCount() - dwStartTickCount > ssStatus.dwWaitHint) {
                break;
            }
        }
    }

    CloseServiceHandle(hService);
    if (ssStatus.dwCurrentState == SERVICE_RUNNING) {
        return ERROR_SUCCESS;
    }
    else {
        //printf("\nService not started. \n");
        //printf("  Current State: %d\n", ssStatus.dwCurrentState);
        //printf("  Exit Code: %d\n", ssStatus.dwWin32ExitCode);
        //printf("  Service Specific Exit Code: %d\n", ssStatus.dwServiceSpecificExitCode);
        //printf("  Check Point: %d\n", ssStatus.dwCheckPoint);
        //printf("  Wait Hint: %d\n", ssStatus.dwWaitHint);
        return ERROR_ACCESS_DENIED;
    }
}

DWORD B1WindowsUtil::stopService(const B1String& serviceName)
{
    SC_HANDLE hManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hManager == NULL) {
        return GetLastError();  //  ERROR_ACCESS_DENIED, ERROR_DATABASE_DOES_NOT_EXIST, ERROR_INVALID_PARAMETER
    }

    SC_HANDLE hService = OpenServiceA(hManager, serviceName.cString(), SERVICE_ALL_ACCESS);
    CloseServiceHandle(hManager);
    if (hService == NULL) {
        return GetLastError();  //  ERROR_ACCESS_DENIED, ERROR_INVALID_NAME, ERROR_SERVICE_DOES_NOT_EXIST
    }

    SERVICE_STATUS_PROCESS ssStatus;
    DWORD dwBytesNeeded;
    if (!QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO, (LPBYTE)&ssStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded)) {
        CloseServiceHandle(hService);
        return GetLastError();  //  ERROR_ACCESS_DENIED, ERROR_INSUFFICIENT_BUFFER
    }
    if (ssStatus.dwCurrentState == SERVICE_STOPPED) {
        CloseServiceHandle(hService);
        return ERROR_SUCCESS;
    }

    SERVICE_STATUS ss;
    if (!ControlService(hService, SERVICE_CONTROL_STOP, &ss)) {
        CloseServiceHandle(hService);
        return GetLastError();  //  ERROR_SERVICE_NOT_ACTIVE
    }

    while (ssStatus.dwCurrentState != SERVICE_STOPPED)
    {
        DWORD dwWaitTime = ssStatus.dwWaitHint / 10;  //  A good interval is one tenth the wait hint.
        if (dwWaitTime < 1000) { //  no less than 1 second.
            dwWaitTime = 1000;
        }
        else if (dwWaitTime > 10000) {  //   no more than 10 seconds.
            dwWaitTime = 10000;
        }
        Sleep(dwWaitTime);

        if (!QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO, (LPBYTE)&ssStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded)) {
            break;
        }
    }

    CloseServiceHandle(hService);
    if (ssStatus.dwCurrentState == SERVICE_STOPPED) {
        return ERROR_SUCCESS;
    }
    else {
        //printf("\nService not started. \n");
        //printf("  Current State: %d\n", ssStatus.dwCurrentState);
        //printf("  Exit Code: %d\n", ssStatus.dwWin32ExitCode);
        //printf("  Service Specific Exit Code: %d\n", ssStatus.dwServiceSpecificExitCode);
        //printf("  Check Point: %d\n", ssStatus.dwCheckPoint);
        //printf("  Wait Hint: %d\n", ssStatus.dwWaitHint);
        return ERROR_ACCESS_DENIED;
    }
}

DWORD B1WindowsUtil::disableWindowsService(const B1String& serviceName)
{
    char command[MAX_PATH] = { 0 };
    DWORD returnValue = 0;
    sprintf_s(command, MAX_PATH, "net stop %s /y", serviceName.cString());
    returnValue = createProcess(command);
    if (returnValue != ERROR_SUCCESS) {
        return returnValue;
    }
    sprintf_s(command, MAX_PATH, "sc config %s start= disabled", serviceName.cString());
    return createProcess(command);
}

bool B1WindowsUtil::isServiceInstalled(const B1String& serviceName)
{
    SC_HANDLE hManager = OpenSCManagerA(NULL, NULL, GENERIC_READ);
    if (hManager == NULL) {
        return false;
    }
    SC_HANDLE hService = OpenServiceA(hManager, serviceName.cString(), GENERIC_READ);
    CloseServiceHandle(hManager);
    if (hService == NULL) {
        return false;
    }
    CloseServiceHandle(hService);
    return true;
}

bool B1WindowsUtil::setProcessHighPriority()
{
    /*
    우선 순위 REALTIME_PRIORITY_CLASS 권장 안함
    REALTIME_PRIORITY_CLASS 
    HIGH_PRIORITY_CLASS
    ABOVE_NORMAL_PRIORITY_CLASS
    NORMAL_PRIORITY_CLASS
    BELOW_NORMAL_PRIORITY_CLASS
    IDLE_PRIORITY_CLASS
    */

    if (!SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS))
    {
        _tprintf(TEXT("Failed to Change Priority mode (%d)\n"), GetLastError());
        return false;
    }

    return true;
}

#endif  //  _WIN32
