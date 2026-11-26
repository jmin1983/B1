//
// B1SystemUtil.cpp
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Base.h"
#include "B1SystemUtil.h"
#include "B1StringUtil.h"
#include "B1Time.h"
#include "B1Thread.h"

#include <sys/stat.h>

#if defined(_WIN32)
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
#else 
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#if defined(__linux__)
#include <sys/reboot.h>
#endif
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <dirent.h>
#include <errno.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <fstream>
#endif

using namespace BnD;

bool B1SystemUtil::isFileExist(const B1String& path)
{
#if defined(_WIN32)
    struct _stat statBuffer;
    return (_stat(path.cString(), &statBuffer) >= 0);
#else
    return (access(path.cString(), F_OK) >= 0);
#endif
}

bool B1SystemUtil::isProcessRunning(const B1String& processName)
{
    return getProcessID(processName) > -1;
}

bool B1SystemUtil::isProcessRunning(int pid)
{
#if defined(_WIN32)
    HANDLE hProcess = OpenProcess(MAXIMUM_ALLOWED, FALSE, pid);
    if (hProcess == NULL) {
        return false;
    }
    CloseHandle(hProcess);
    return true;
#else
    if (kill(pid, 0) != 0) {
        return false;
    }
    bool isZombie = false;
    {
        B1String temp(B1String::formatAs("/proc/%d/stat", pid));
        FILE* fpstat = fopen(temp.cString(), "r");
        if (fpstat) {
            int rpid = 0;
            char rcmd[32];
            char rstatc = 0;
            if (fscanf(fpstat, "%d %30s %c", &rpid, rcmd, &rstatc) > 0) {
                isZombie = rstatc == 'Z';
            }
            fclose(fpstat);
        }
    }
    return isZombie != true;
#endif
}

bool B1SystemUtil::createDirectory(const B1String& path)
{
    if (path.isEmpty()) {
        return false;
    }
#if defined(_WIN32)
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
#else
    B1String opath = path.copy();
    int32 len = opath.length();
    if (opath[len - 1] == '/') {
        opath[len - 1] = '\0';
    }

    for (int32 i = 0; i < len; i++) {
        if (opath[i] == '/') {
            opath[i] = '\0';
            if (i == 0 || (i > 0 && opath[i - 1] == '/')) {
                // in case of the consecutive '/' path delimiter
                // do nothing
            }
            else if (access(opath.cString(), R_OK | X_OK) < 0) {
                if (mkdir(opath.cString(), 0755) < 0) {
                    return false;
                }
            }
            opath[i] = '/';
        }
    }
    if (access(opath.cString(), R_OK | X_OK) < 0) {
        if (mkdir(opath.cString(), 0755) < 0) {
            return false;
        }
    }
#endif
    return true;
}

bool B1SystemUtil::deleteDirectory(const B1String& path)
{
    if (path.isEmpty()) {
        return false;
    }
#if defined(_WIN32)
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
#else
    struct dirent* result;
    DIR* dir = opendir(path.cString());
    if (dir == nullptr) {
        return false;
    }
    while (true) {
        if (!(result = readdir(dir))) {
            break;  // no more file
        }
        B1String fname(result->d_name);
        if (fname == "." || fname == "..") {
            continue;
        }
        B1String name = path + "/" + fname;
        if (result->d_type == DT_DIR) {
            if (!deleteDirectory(name)) {
                closedir(dir);
                return false;
            }
        }
        else {
            if (remove(name.cString()) < 0) {
                closedir(dir);
                return false;
            }
        }
    }
    closedir(dir);
    return (remove(path.cString()) >= 0);
#endif
}

uint32 B1SystemUtil::findFiles(const B1String& directoryPath, const B1String& fileExt, std::list<B1String>* resultFilenames)
{
    B1String path = directoryPath.copy();
    B1StringUtil::removeLastPathSeparator(&path);
    uint32 count = 0;

#if defined(_WIN32)
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
    do {
        count++;
        if (resultFilenames) {
            resultFilenames->push_back(B1String(fd.cFileName));
        }
    } while (::FindNextFileA(handle, &fd));
    ::FindClose(handle);
#else
    struct dirent* result;
    DIR* dir = opendir(path.cString());
    while (dir) {
        if (!(result = readdir(dir))) {
            break;  // no more file
        }
        B1String pathName;
        pathName.from(result->d_name);
        if (fileExt.isEmpty() || checkFileExtension(pathName, fileExt)) {
            count++;
            if (resultFilenames) {
                resultFilenames->push_back(std::move(pathName));
            }
        }
    }
    if (dir) {
        closedir(dir);
    }
#endif
    return count;
}

bool B1SystemUtil::copyFile(const B1String& dstPath, const B1String& srcPath, bool replaceIfExist)
{
#if defined(_WIN32)
    return ::CopyFileA(srcPath.cString(), dstPath.cString(), replaceIfExist != true) ? true : false;
#else
    if (replaceIfExist != true && access(dstPath.cString(), R_OK) == 0) {
        return false;
    }
    char buf[1024];
    FILE* inF = fopen(srcPath.cString(), "rb");
    if (inF == nullptr) {
        return false;
    }
    FILE* outF = fopen(dstPath.cString(), "wb");
    if (outF == nullptr) {
        fclose(inF);
        return false;
    }
    bool ret = true;
    uint32 cnt = fread(buf, 1, 1024, inF);
    while (cnt == 1024) {
        if (fwrite(buf, 1, cnt, outF) < cnt) {
            ret = false;
            break;
        }
        cnt = fread(buf, 1, 1024, inF);
    }
    if (ret && cnt > 0) {
        if (fwrite(buf, 1, cnt, outF) < cnt) {
            ret = false;
        }
    }
    if (ret && feof(inF) == 0) {
        ret = false;
    }
    fclose(inF);
    fclose(outF);
    return ret;
#endif
}

bool B1SystemUtil::moveFile(const B1String& dstPath, const B1String& srcPath, bool replaceIfExist)
{
#if defined(_WIN32)
    DWORD flags = MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH;
    if (replaceIfExist) {
        flags |= MOVEFILE_REPLACE_EXISTING;
    }
    return TRUE == ::MoveFileExA(srcPath.cString(), dstPath.cString(), flags);
#else
    if (replaceIfExist != true && access(dstPath.cString(), R_OK) == 0) {
        return false;
    }
    bool ret = true;
    if (rename(srcPath.cString(), dstPath.cString()) != 0) {
        if (errno == EXDEV) { // other partition, so copy and delete
            ret = copyFile(dstPath, srcPath, replaceIfExist);
            ret = ret && deleteFile(srcPath);
        }
        else {
            ret = false;
        }
    }
    return ret;
#endif
}

bool B1SystemUtil::deleteFile(const B1String& path)
{
#if defined(_WIN32)
    return ::DeleteFileA(path.cString()) ? true : false;
#else
    return ::remove(path.cString()) == 0;
#endif
}

bool B1SystemUtil::checkFileExtension(const B1String& path, const B1String& fileExt)
{
#if defined(_WIN32)
    B1String ext = fileExtensionOnly(path);
    return fileExtensionOnly(path).caseInsensitiveCompare(fileExt);
#else
    return fileExtensionOnly(path) == fileExt;
#endif
}

bool B1SystemUtil::fileLastModifiedTime(const B1String& path, int64* modifiedTime)
{
    struct stat buffer;
    int result = stat(path.cString(), &buffer);
    if (result != 0) {
        return false;
    }
    *modifiedTime = buffer.st_mtime;
    return true;
}

B1String B1SystemUtil::fileExtensionOnly(const B1String& path)
{
#if defined(_WIN32)
    uint32 i = path.findLastOf(".");
    return (i == B1String::NPOS) ? B1String("") : path.substring(i + 1);
#else
    if (path == "." || path == "..") {
        return B1String("");
    }
    uint32 i = path.findLastOf(".");
    return (i == B1String::NPOS) ? B1String("") : path.substring(i + 1);
#endif
}

B1String B1SystemUtil::removeFileExtension(const B1String& path)
{
#if defined(_WIN32)
    uint32 i = path.findLastOf(".");
    return (i == B1String::NPOS) ? B1String("") : path.substring(0, i);
#else
    if (path == "." || path == "..") {
        return B1String("");
    }
    uint32 i = path.findLastOf(".");
    return (i == B1String::NPOS) ? B1String("") : path.substring(0, i);
#endif
}

B1String B1SystemUtil::getModuleFileName()
{
    char buffer[1024] = {0};
#if defined(_WIN32)
    ::GetModuleFileNameA(NULL, buffer, 1024);
    return B1String(buffer);
#else
    int32 bytes = -1;
    if ((bytes = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1)) < 0) {
        return B1String("");
    }
    buffer[bytes] = '\0';
    return B1String(buffer);
#endif
}

B1String B1SystemUtil::getFileNameOnly()
{
    return B1StringUtil::lastPathComponent(getModuleFileName());
}

B1String B1SystemUtil::getCurrentDirectory()
{
    B1String path = getModuleFileName();
    B1StringUtil::removeLastPathComponent(&path);
    return path;
}

B1String B1SystemUtil::getSystemID()
{
#if defined(_WIN32)
    HW_PROFILE_INFOA info;
    if (::GetCurrentHwProfileA(&info) != TRUE) {
        return "";
    }
    return B1String(info.szHwProfileGuid);
#else
    FILE* file = fopen("/sys/class/dmi/id/product_uuid", "r");
    if (nullptr == file) {
        file = fopen("/var/lib/dbus/machine-id", "r");
        if (nullptr == file) {
            return "";
        }
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    std::vector<char> buffer(size + 1, 0);
    if (0 == fread(&buffer[0], sizeof(char), size, file)) {
        fclose(file);
        return "";
    }
    fclose(file);
    B1String result(&buffer[0]);
    result.trim();
    return result;
#endif
}

int B1SystemUtil::getProcessID(const B1String& processName)
{
#if defined(_WIN32)
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
#else
    int pid = -1;

    // Open the /proc directory
    DIR* dp = opendir("/proc");
    if (dp != nullptr) {
        // Enumerate all entries in directory until process found.
        struct dirent* dirp;
        while (pid < 0 && (dirp = readdir(dp))) {
            // Skip non-numeric entries
            int id = atoi(dirp->d_name);
            if (id > 0) {
                // Read contents of virtual /proc/{pid}/cmdline file.
                std::string cmdPath = std::string("/proc/") + dirp->d_name + "/cmdline";
                std::ifstream cmdFile(cmdPath.c_str());
                std::string cmdLine;
                getline(cmdFile, cmdLine);
                if (!cmdLine.empty())
                {
                    // Keep first cmdline item which contains the program path.
                    size_t pos = cmdLine.find('\0');
                    if (pos != std::string::npos)
                        cmdLine = cmdLine.substr(0, pos);
                    // Keep program name only, removing the path.
                    pos = cmdLine.rfind('/');
                    if (pos != std::string::npos)
                        cmdLine = cmdLine.substr(pos + 1);
                    // Compare against requested process name.
                    if (processName == B1String(std::move(cmdLine)))
                        pid = id;
                }
            }
        }
        closedir(dp);
    }
    return pid;
#endif
}

uint32 B1SystemUtil::getCurrentProcessID()
{
#if defined(_WIN32)
    return static_cast<uint32>(::GetCurrentProcessId());
#else
    return getpid();
#endif
}

uint32 B1SystemUtil::getCurrentThreadID()
{
#if defined(_WIN32)
    return static_cast<uint32>(::GetCurrentThreadId());
#else
    return static_cast<uint32>(::pthread_self());
#endif
}

uint32 B1SystemUtil::createProcess(const B1String& process, int* createdPid)
{
#if defined(_WIN32)
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
#else
    if (isFileExist(process) != true) {
        return ENOENT;
    }

    int pid = fork();

    if (pid < 0) {  // parent process. but child couldn't be created.
        return errno;
    }
    else if (pid == 0) {    // child process.
        execl(process.cString(), process.cString(), nullptr);

        // If execution reaches this point, execl has failed.
        _exit(errno);
    }
    else if (pid > 0) { // parent process.
        int status;
        waitpid(pid, &status, WNOHANG);
        if (createdPid) {
            *createdPid = pid;
        }
        return 0;
    }
    return 0;
#endif
}

uint32 B1SystemUtil::createProcessArg(const B1String& process, const B1String& arg, int* createdPid)
{
#if defined(_WIN32)
    std::vector<B1String> args(1);
    args[0] = arg.copy();
    return createProcessArgs(process, args, createdPid);
#else
    std::vector<B1String> args(1);
    args[0] = arg.copy();
    return createProcessArgs(process, args, createdPid);
#endif
}

uint32 B1SystemUtil::createProcessArgs(const B1String& process, const std::vector<B1String>& args, int* createdPid)
{
#if defined(_WIN32)
    B1String temp;
    temp.format("\"%s\"", process.cString());
    for (const auto& arg : args) {
        temp.appendf(" \"%s\"", arg.cString());
    }
    return createProcess(temp, createdPid);
#else
    if (isFileExist(process) != true) {
        return ENOENT;
    }

    int pid = fork();
    if (pid < 0) {  // parent process. but child couldn't be created.
        return errno;
    }
    else if (pid == 0) {    // child process.
        // Create a char* array for execv().
        // Capacity required: process name (1) + number of arguments (N) + NULL terminator (1).
        std::vector<char*> exec_args;
        exec_args.reserve(args.size() + 2);

        // argv[0] conventionally holds the name (path) of the program being executed.
        // const_cast is required to meet the POSIX execv signature: int execv(const char *path, char *const argv[]);
        exec_args.push_back(const_cast<char*>(process.cString()));

        // argv[1] ~ argv[N]: Actual arguments.
        for (size_t i = 0; i < args.size(); ++i) {
            exec_args.push_back(const_cast<char*>(args[i].cString()));
        }

        // The last element of the argv array MUST be a nullptr. 
        // The kernel uses this to identify the end of the argument array.
        exec_args.push_back(nullptr);

        // Call execv: If successful, the process is replaced and does not return.
        execv(process.cString(), exec_args.data());

        // If execution reaches this point, execv has failed. 
        // Use _exit() instead of return to prevent the child process from returning to the caller.
        _exit(errno);
    }
    else if (pid > 0) { // parent process.
        int status;
        waitpid(pid, &status, WNOHANG);
        if (createdPid) {
            *createdPid = pid;
        }
        return 0;
    }
    return 0;
#endif
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
#if defined(_WIN32)
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
#else
    int result = kill(pid, SIGINT); //  Assume that all processes other than Linux Services will be killed when SIGINT is received.
    if (result != 0) {
        return result;
    }
    for (int32 i = 0; i < 1000; ++i) {
        if (isProcessRunning(pid) != true) {
            return 0;
        }
        B1Thread::sleep(10);
    }
    result = kill(pid, 9);
    B1Thread::sleep(50);
    return result;
#endif
}

bool B1SystemUtil::rebootSystem()
{
#if defined(_WIN32)
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
#else
#if defined(__linux__)
    sync();
    int result = reboot(RB_AUTOBOOT);
    return 0 == result;
#else
    return false;
#endif
#endif
}

bool B1SystemUtil::dropCaches()
{
#if defined(_WIN32)
    return true;    //  not supported.
#else
    bool result = false;
    sync();
    int fd = open("/proc/sys/vm/drop_caches", O_WRONLY);
    if (fd > -1) {
        if (write(fd, "3", sizeof(char)) > -1) {
            result = true;
        }
        close(fd);
    }
    return result;
#endif
}

bool B1SystemUtil::getLocalNetworkAddresses(std::list<B1String>* addresses)
{
#if defined(_WIN32)
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
#else
    struct ifaddrs* addrs, * tmp;
    getifaddrs(&addrs);
    tmp = addrs;
    while (tmp != 0) {
        if (tmp->ifa_addr != 0 && tmp->ifa_addr->sa_family == AF_INET && (strcmp("lo", tmp->ifa_name) != 0 && (tmp->ifa_flags & (IFF_RUNNING)))) {
            char host[NI_MAXHOST];
            if (getnameinfo(tmp->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, nullptr, 0, NI_NUMERICHOST) == 0) {
                addresses->push_back(std::move(B1String(host)));
            }
        }
        tmp = tmp->ifa_next;
    }
    freeifaddrs(addrs);
    return addresses->empty();
#endif
}

bool B1SystemUtil::getHostName(B1String* name)
{
    char hostName[128] = {0};
    if (::gethostname(hostName, sizeof(hostName)) != 0) {
        return false;
    }
    try {
        name->from(hostName);
    }
    catch (...) {
        return false;
    }
    return true;
}

bool B1SystemUtil::isSystemWindows()
{
#if defined(_WIN32)
    return true;
#else
    return false;
#endif
}

bool B1SystemUtil::isSystemLinux()
{
#if defined(__linux__)
    return true;
#else
    return false;
#endif
}

bool B1SystemUtil::isSystemX86()
{
#if defined(__i386__) || defined(_M_IX86)
    return true;
#else
    return false;
#endif
}

bool B1SystemUtil::isSystemX64()
{
#if defined(__x86_64__) || defined(_M_X64)
    return true;
#else
    return false;
#endif
}

bool B1SystemUtil::isSystemARM32()
{
#if defined(__arm__) || defined(_M_ARM)
    return true;
#else
    return false;
#endif
}

bool B1SystemUtil::isSystemARM64()
{
#if defined(__aarch64__) || defined(_M_ARM64)
    return true;
#else
    return false;
#endif
}
