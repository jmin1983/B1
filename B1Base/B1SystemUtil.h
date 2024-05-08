//
// B1SystemUtil.h
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1BASE_SYSTEM_UTIL_H
#define _B1BASE_SYSTEM_UTIL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Base/B1String.h>
#include <list>

namespace BnD {
    namespace B1SystemUtil {
        bool isFileExist(const B1String& path);
        bool isProcessRunning(const B1String& processName);
        bool createDirectory(const B1String& path);
        bool deleteDirectory(const B1String& path);
        uint32 findFiles(const B1String& directoryPath, const B1String& fileExt, std::list<B1String>* resultFilenames = NULL);
        bool copyFile(const B1String& dstPath, const B1String& srcPath, bool replaceIfExist = false);
        bool moveFile(const B1String& dstPath, const B1String& srcPath, bool replaceIfExist = false);
        bool deleteFile(const B1String& path);
        bool checkFileExtension(const B1String& path, const B1String& fileExt);
        B1String fileExtensionOnly(const B1String& path);
        B1String removeFileExtension(const B1String& path);
        B1String getModuleFileName();
        B1String getFileNameOnly();
        B1String getCurrentDirectory();
        B1String getSystemID();
        int getProcessID();
        int getProcessID(const B1String& processName);
        uint32 getCurrentThreadID();
        uint32 createProcess(const B1String& process);
        uint32 createProcessArg(const B1String& process, const B1String& arg);
        uint32 createProcessArgs(const B1String& process, const std::vector<B1String>& args);   //  max args_size is 5.
        int terminateProcess(const B1String& processName);
        bool rebootSystem();
        bool getLocalNetworkAddresses(std::list<B1String>* addresses);
    }
}   //  !BnD

#endif  // !_B1BASE_SYSTEM_UTIL_H
