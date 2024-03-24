//
// B1FileLog.h
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1BASE_FILE_LOG_H
#define _B1BASE_FILE_LOG_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Base/B1Condition.h>
#include <B1Base/B1Lock.h>
#include <B1Base/B1Looper.h>
#include <B1Base/B1String.h>

#include <list>

namespace BnD {
    class B1FileLog : protected B1Looper {
    public:
        B1FileLog();
        virtual ~B1FileLog();
    private:
        FILE* _fp;
        int32 _lastDay;
        int32 _lastHour;
    protected:
        enum {
            MAX_LOG_COUNT = 10,
        };
    protected:
        bool _started;
        B1String _dirPath;
        B1String _fileName;
        B1String _fileExtension;
        int32 _maxRemainFileCount;
        std::list<B1String> _logs;
        B1Condition _logWait;
        B1RecursiveLock _logLock;
    private:
        void writeLogToFile(const std::list<B1String> &logs);
        void getDeprecatedFiles(std::list<B1String> *deprecatedFiles);
    protected:
        void implLooperFunc() final;
    public:
        bool start(B1String&& dirPath, B1String&& fileName, int32 maxRemainFileCount = 24 * 10, B1String&& fileExtension = "log");
        void stop();
        bool testWriteLogFile();
        bool write(B1String&& log);
        void flush();
        void deleteDeprecatedFiles();
    };
}   //  !BnD

#endif  //  !_B1BASE_FILE_LOG_H
