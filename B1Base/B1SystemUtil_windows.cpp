//
// B1SystemUtil_windows.cpp
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Base.h"

#if defined(WIN32)

#include "B1SystemUtil.h"
#include "B1StringUtil.h"
#include "B1Time.h"
#include "B1Thread.h"

#include <windows.h>
#include <Dbghelp.h>
#include "iphlpapi.h"
#if defined(UNICODE)
#undef UNICODE
#include <Tlhelp32.h>
#define UNICODE
#else
#include <Tlhelp32.h>
#endif
#pragma comment (lib, "Dbghelp.lib")
#pragma comment (lib, "Iphlpapi.lib")

using namespace BnD;

bool B1SystemUtil::isFileExist(const B1String& path)
{
    struct _stat statBuffer;
    return (_stat(path.cString(), &statBuffer) >= 0);
}

bool B1SystemUtil::isProcessRunning(const B1String& processName)
{
    return getProcessID(processName) > -1;
}

bool B1SystemUtil::isProcessRunning(int pid)
{
    HANDLE hProcess = OpenProcess(MAXIMUM_ALLOWED, FALSE, pid);
    if (hProcess == NULL) {
        return false;
    }
    CloseHandle(hProcess);
    return true;
}

bool B1SystemUtil::createDirectory(const B1String& path)
{
    B1String opath = path.copy();
    int32 len = opath.length();
    if (opath[len - 1] == '\\' || opath[len - 1] == '/') {
        opath[len - 1] = '\0';
    }
    struct _stat statBuffer;
    for (int32 i = 0; i < len; i++) {
        if (opath[i] == '\\' || opath[i] == '/') {
            opath[i] = '\0';
            if (i == 0 || (i > 0 && (opath[i - 1] == ':' || opath[i - 1] == '\\'))) {
                //  noop.
            }
            else if (_stat(opath.cString(), &statBuffer) < 0) {
                if (::CreateDirectoryA(opath.cString(), 0) == 0) {
                    return false;
                }
            }
            opath[i] = '\\';
        }
    }
    if (_stat(opath.cString(), &statBuffer) < 0) {
        if (::CreateDirectoryA(opath.cString(), 0) == 0) {
            return false;
        }
    }
    return true;
}

bool B1SystemUtil::deleteDirectory(const B1String& path)
{
    if (path.isEmpty()) {
        return false;
    }

    B1String filter(path + "\\*");
    WIN32_FIND_DATAA fd;
    HANDLE handle = ::FindFirstFileA(filter.cString(), &fd);
    if (INVALID_HANDLE_VALUE == handle) {
        return false;
    }
    do {
        B1String fname(fd.cFileName);
        if (fname == "." || fname == "..") {
            continue;
        }
        B1String name = path + "\\" + fname;
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (!deleteDirectory(name)) {
                return false;
            }
        }
        else {
            if (TRUE != ::DeleteFileA(name.cString())) {
                return false;
            }
        }
    } while (::FindNextFileA(handle, &fd));
    ::FindClose(handle);
    return (0 != ::RemoveDirectoryA(path.cString()));
}

uint32 B1SystemUtil::findFiles(const B1String& directoryPath, const B1String& fileExt, std::list<B1String>* resultFilenames)
{
    B1String path = directoryPath.copy();
    B1StringUtil::removeLastPathSeparator(&path);

    B1String filter(path + "\\*");
    if (!fileExt.isEmpty()) {
        filter.appendf(".%s", fileExt.cString());
    }

    WIN32_FIND_DATAA fd;
    HANDLE handle = ::FindFirstFileA(filter.cString(), &fd);
    if (INVALID_HANDLE_VALUE == handle) {
        return 0;
    }
    if (resultFilenames) {
        resultFilenames->clear();
    }
    uint32 count = 0;
    do {
        count++;
        if (resultFilenames) {
            resultFilenames->push_back(B1String(fd.cFileName));
        }
    } while (::FindNextFileA(handle, &fd));
    ::FindClose(handle);
    return count;
}

bool B1SystemUtil::copyFile(const B1String& dstPath, const B1String& srcPath, bool replaceIfExist)
{
    return ::CopyFileA(srcPath.cString(), dstPath.cString(), replaceIfExist != true) ? true : false;
}

bool B1SystemUtil::moveFile(const B1String& dstPath, const B1String& srcPath, bool replaceIfExist)
{
    DWORD flags = MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH;
    if (replaceIfExist) {
        flags |= MOVEFILE_REPLACE_EXISTING;
    }
    return TRUE == ::MoveFileExA(srcPath.cString(), dstPath.cString(), flags);
}

bool B1SystemUtil::deleteFile(const B1String& path)
{
    return ::DeleteFileA(path.cString()) ? true : false;
}

bool B1SystemUtil::checkFileExtension(const B1String& path, const B1String& fileExt)
{
    B1String ext = fileExtensionOnly(path);
    return fileExtensionOnly(path).caseInsensitiveCompare(fileExt);
}

B1String B1SystemUtil::fileExtensionOnly(const B1String& path)
{
    uint32 i = path.findLastOf(".");
    return (i == B1String::NPOS) ? B1String("") : path.substring(i + 1);
}

B1String B1SystemUtil::removeFileExtension(const B1String& path)
{
    uint32 i = path.findLastOf(".");
    return (i == B1String::NPOS) ? B1String("") : path.substring(0, i);
}

B1String B1SystemUtil::getModuleFileName()
{
    char buffer[1024] = { 0 };
    ::GetModuleFileNameA(NULL, buffer, 1024);
    return B1String(buffer);
}

B1String B1SystemUtil::getFileNameOnly()
{
    return B1StringUtil::lastPathComponent(getModuleFileName());
}

B1String B1SystemUtil::getSystemID()
{
    HW_PROFILE_INFOA info;
    if (::GetCurrentHwProfileA(&info) != TRUE) {
        return "";
    }
    return B1String(info.szHwProfileGuid);
}

int B1SystemUtil::getProcessID(const B1String& processName)
{
    HANDLE processSnap;
    PROCESSENTRY32 pe32;
    processSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);  // Take a snapshot of all processes in the system.
    if (processSnap == INVALID_HANDLE_VALUE)
        return -1;
    pe32.dwSize = sizeof(PROCESSENTRY32);   // Set the size of the structure before using it.
    if (!Process32First(processSnap, &pe32)) {  // Retrieve information about the first process, and exit if unsuccessful
        CloseHandle(processSnap);     // Must clean up the snapshot object!
        return -1;
    }

    do {    // Now walk the snapshot of processes, and display information about each process in turn
        if (strcmp(processName.cString(), pe32.szExeFile))
            continue;
        else {
            CloseHandle(processSnap);
            return pe32.th32ProcessID;
        }
    } while (Process32Next(processSnap, &pe32));

    CloseHandle(processSnap);
    return -1;
}

uint32 B1SystemUtil::getCurrentProcessID()
{
    return static_cast<uint32>(::GetCurrentProcessId());
}

uint32 B1SystemUtil::getCurrentThreadID()
{
    return static_cast<uint32>(::GetCurrentThreadId());
}

uint32 B1SystemUtil::createProcess(const B1String& process, int* createdPid)
{
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    if (::CreateProcessA(NULL, const_cast<char*>(process.cString()), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi) != TRUE) {
        return GetLastError();
    }
    if (createdPid) {
        *createdPid = pi.dwProcessId;
    }
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return ERROR_SUCCESS;
}

uint32 B1SystemUtil::createProcessArg(const B1String& process, const B1String& arg, int* createdPid)
{
    std::vector<B1String> args(1);
    args[0] = arg.copy();
    return createProcessArgs(process, args, createdPid);
}

uint32 B1SystemUtil::createProcessArgs(const B1String& process, const std::vector<B1String>& args, int* createdPid)
{
    B1String temp;
    temp.format("\"%s\"", process.cString());
    for (const auto& arg : args) {
        temp.appendf(" \"%s\"", arg.cString());
    }
    return createProcess(temp, createdPid);
}

int B1SystemUtil::terminateProcess(const B1String& processName)
{
    int pid = B1SystemUtil::getProcessID(processName);
    if (pid < 0) {
        return ENOENT;
    }
    return terminateProcess(pid);
}

int B1SystemUtil::terminateProcess(int pid)
{
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
    DWORD wait = WaitForSingleObject(hProcess, 10 * 1000);
    if (wait == WAIT_FAILED) {
        CloseHandle(hProcess);
        return GetLastError();
    }
    if (wait == WAIT_TIMEOUT) {
        CloseHandle(hProcess);
        return WAIT_TIMEOUT;
    }
    CloseHandle(hProcess);
    return ERROR_SUCCESS;
}

bool B1SystemUtil::rebootSystem()
{
    HANDLE hToken;          // handle to process token
    TOKEN_PRIVILEGES tkp;   // pointer to token structure

    // Get the current process token handle so we can get shutdown privilege.
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        return false;

    // Get the LUID for shutdown privilege.
    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);

    tkp.PrivilegeCount = 1;  // one privilege to set
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    // Get shutdown privilege for this process.
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

    // Cannot test the return value of AdjustTokenPrivileges.
    DWORD djustTokenPrivilegesResult = GetLastError();
    if (djustTokenPrivilegesResult != ERROR_SUCCESS) {
        return false;
    }

    // Display the shutdown dialog box and start the countdown.
    if (!InitiateSystemShutdownEx(NULL, NULL, 0, FALSE, TRUE, SHTDN_REASON_MAJOR_APPLICATION)) {
        return false;
    }

    // Disable shutdown privilege.
    tkp.Privileges[0].Attributes = 0;
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

    return true;
}

bool B1SystemUtil::dropCaches()
{
    return true;
}

bool B1SystemUtil::getLocalNetworkAddresses(std::list<B1String>* addresses)
{
    addresses->clear();
    const size_t maxInterfaceCount = 10;
    IP_ADAPTER_INFO adapterInfo[maxInterfaceCount];
    DWORD dwBufLen = sizeof(adapterInfo);
    if (GetAdaptersInfo(adapterInfo, &dwBufLen) == ERROR_SUCCESS) {
        IP_ADAPTER_INFO* info = &adapterInfo[0];
        while (info) {
            PIP_ADDR_STRING ipAddress = &info->IpAddressList;
            while (ipAddress) {
                B1String address(ipAddress->IpAddress.String);
                if (address != "0.0.0.0") {
                    addresses->push_back(std::move(address));
                }
                ipAddress = ipAddress->Next;
            }
            info = info->Next;
        }
    }
    return addresses->empty();
}

#endif
