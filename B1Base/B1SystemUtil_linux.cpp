//
// B1SystemUtil_linux.cpp
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Base.h"

#if !defined(_WIN32)

#include "B1SystemUtil.h"
#include "B1StringUtil.h"
#include "B1Time.h"
#include "B1Thread.h"

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#if defined(__linux__)
#include <sys/reboot.h>
#endif
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <fstream>

using namespace BnD;

bool B1SystemUtil::isFileExist(const B1String& path)
{
    return (access(path.cString(), F_OK) >= 0);
}

bool B1SystemUtil::isProcessRunning(const B1String& processName)
{
    return getProcessID(processName) > -1;
}

bool B1SystemUtil::createDirectory(const B1String& path)
{
    B1String opath = path;
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
    return true;
}

bool B1SystemUtil::deleteDirectory(const B1String& path)
{
    if (path.isEmpty()) {
        return false;
    }

    struct dirent* result;
    DIR* dir = opendir(path.cString());
    if (dir == NULL) {
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
}

uint32 B1SystemUtil::findFiles(const B1String& directoryPath, const B1String& fileExt, std::list<B1String>* resultFilenames)
{
    B1String path = directoryPath.copy();
    B1StringUtil::removeLastPathSeparator(&path);

    uint32 count = 0;
    B1String pathName;
    struct dirent* result;
    DIR* dir = opendir(path.cString());
    while (dir) {
        if (!(result = readdir(dir))) {
            break;  // no more file
        }
        pathName.from(result->d_name);
        if (fileExt.isEmpty() || checkFileExtension(pathName, fileExt)) {
            count++;
            if (resultFilenames) {
                resultFilenames->push_back(pathName);
            }
        }
    }
    if (dir) {
        closedir(dir);
    }
    return count;
}

bool B1SystemUtil::copyFile(const B1String& dstPath, const B1String& srcPath, bool replaceIfExist)
{
    if (replaceIfExist != true && access(dstPath.cString(), R_OK) == 0) {
        return false;
    }
    char buf[1024];
    FILE* inF = fopen(srcPath.cString(), "rb");
    if (inF == NULL) {
        return false;
    }
    FILE* outF = fopen(dstPath.cString(), "wb");
    if (outF == NULL) {
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
}

bool B1SystemUtil::moveFile(const B1String& dstPath, const B1String& srcPath, bool replaceIfExist)
{
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
}

bool B1SystemUtil::deleteFile(const B1String& path)
{
    return ::remove(path.cString()) == 0;
}

bool B1SystemUtil::checkFileExtension(const B1String& path, const B1String& fileExt)
{
    return fileExtensionOnly(path) == fileExt;
}

B1String B1SystemUtil::fileExtensionOnly(const B1String& path)
{
    if (path == "." || path == "..") {
        return B1String("");
    }
    uint32 i = path.findLastOf(".");
    return (i == B1String::NPOS) ? B1String("") : path.substring(i + 1);
}

B1String B1SystemUtil::removeFileExtension(const B1String& path)
{
    if (path == "." || path == "..") {
        return B1String("");
    }
    uint32 i = path.findLastOf(".");
    return (i == B1String::NPOS) ? B1String("") : path.substring(0, i);
}

B1String B1SystemUtil::getModuleFileName()
{
    char buffer[1024] = { 0 };
    int32 bytes = -1;
    if ((bytes = readlink("/proc/self/exe", buffer, 1024)) < 0) {
        return B1String("");
    }
    buffer[bytes] = '\0';
    return B1String(buffer);
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
    FILE* file = fopen("/sys/class/dmi/id/product_uuid", "r");
    if (NULL == file) {
        file = fopen("/var/lib/dbus/machine-id", "r");
        if (NULL == file) {
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
}

int B1SystemUtil::getProcessID()
{
    return getpid();
}

int B1SystemUtil::getProcessID(const B1String& processName)
{
    int pid = -1;

    // Open the /proc directory
    DIR* dp = opendir("/proc");
    if (dp != NULL) {
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
                    if (processName == B1String(cmdLine))
                        pid = id;
                }
            }
        }
    }
    closedir(dp);
    return pid;
}

uint32 B1SystemUtil::createProcess(const B1String& process)
{
    if (isFileExist(process) != true)
        return ENOENT;
    int pid = fork();
    if (pid < 0)
        return errno;
    else if (pid == 0) {
        if (execl(process.cString(), process.cString(), NULL) == -1)
            return errno;
        exit(0);
    }
    else if (pid > 0) {
        int status;
        waitpid(pid, &status, WNOHANG);
        return 0;
    }
    return 0;
}

uint32 B1SystemUtil::createProcessArg(const B1String& process, const B1String& arg)
{
    std::vector<B1String> args(1);
    args[0] = arg;
    return createProcessArgs(process, args);
}

uint32 B1SystemUtil::createProcessArgs(const B1String& process, const std::vector<B1String>& args)
{
    if (isFileExist(process) != true)
        return ENOENT;
    int pid = fork();
    if (pid < 0)
        return errno;
    else if (pid == 0) {
        int result = -1;
        if (args.empty())
            result = execl(process.cString(), process.cString(), NULL);
        else if (args.size() == 1)
            result = execl(process.cString(), process.cString(), args[0].cString(), NULL);
        else if (args.size() == 2)
            result = execl(process.cString(), process.cString(), args[0].cString(), args[1].cString(), NULL);
        else if (args.size() == 3)
            result = execl(process.cString(), process.cString(), args[0].cString(), args[1].cString(), args[2].cString(), NULL);
        else if (args.size() == 4)
            result = execl(process.cString(), process.cString(), args[0].cString(), args[1].cString(), args[2].cString(), args[3].cString(), NULL);
        else if (args.size() == 5)
            result = execl(process.cString(), process.cString(), args[0].cString(), args[1].cString(), args[2].cString(), args[3].cString(), args[4].cString(), NULL);
        else {
            return E2BIG;
        }
        if (-1 == result)
            return errno;
        exit(0);
    }
    else if (pid > 0) {
        int status;
        waitpid(pid, &status, WNOHANG);
        return 0;
    }
    return 0;
}

int B1SystemUtil::terminateProcess(const B1String& processName)
{
    int pid = B1SystemUtil::getProcessID(processName);
    if (pid < 0)
        return ENOENT;

    int result = kill(pid, SIGINT); //  linux service가 아닌 모든 프로세스는 SIGINT를 받으면 종료된다고 가정.
    if (result != 0)
        return result;
    for (int32 i = 0; i < 1000; ++i) {
        if (B1SystemUtil::getProcessID(processName) == -1)
            return 0;
        B1Thread::sleep(10);
    }
    result = kill(pid, 9);
    B1Thread::sleep(50);
    return result;
}

bool B1SystemUtil::rebootSystem()
{
#if defined(__linux__)
    sync();
    int result = reboot(RB_AUTOBOOT);
    return 0 == result;
#else
    return false;
#endif
}

#endif
