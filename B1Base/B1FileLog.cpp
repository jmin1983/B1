//
// B1FileLog.cpp
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Base.h"
#include "B1FileLog.h"
#include "B1String.h"
#include "B1SystemUtil.h"
#include "B1Time.h"

#include <set>

#include <stdio.h>

using namespace BnD;

B1FileLog::B1FileLog()
    : B1Looper()
    , _fp(NULL)
    , _lastDay(-1)
    , _lastHour(-1)
    , _started(false)
    , _dirPath()
    , _fileName()
    , _fileExtension()
    , _maxRemainFileCount(-1)
    , _logs()
    , _logLock()
{
}

B1FileLog::~B1FileLog()
{
    stop();
}

void B1FileLog::writeLogToFile(const std::list<B1String> &logs)
{
    const B1Time current = B1Time::currentTime();
    int32 currentDay = current.day();
    int32 currentHour = current.hour();
    if (_lastDay != currentDay ||
        _lastHour != currentHour) {
        _lastDay = currentDay;
        _lastHour = currentHour;
        B1String filePath;
        filePath.format("%s/%s_%04d%02d%02d%02d.%s",
                        _dirPath.cString(),
                        _fileName.cString(),
                        current.year(), current.month(), currentDay, currentHour,
                        _fileExtension.cString());
        if (_fp) {
            fclose(_fp);
        }
        deleteDeprecatedFiles();
        _fp = fopen(filePath.cString(), "a");
    }
    if (_fp == NULL) {
        _lastDay = -1;
        _lastHour = -1;
        return;
    }
    for (const auto& log : logs) {
        fprintf(_fp, "%s", log.cString());
#if defined(_DEBUG)
        printf("%s", log.cString());
#endif
    }
    fflush(_fp);
}

void B1FileLog::getDeprecatedFiles(std::list<B1String> *deprecatedFiles)
{
    std::list<B1String> files;
    int32 total = B1SystemUtil::findFiles(_dirPath, _fileExtension, &files);
    int32 deprecatedSize = total - _maxRemainFileCount;
    if (deprecatedSize <= 0) {
        return;
    }
    std::set<B1String> orderedFiles(files.begin(), files.end());
    for (const auto& orderedFile : orderedFiles) {
        deprecatedFiles->push_back(_dirPath + "/" + orderedFile);
        if (--deprecatedSize <= 0) {
            break;
        }
    }
}

void B1FileLog::implLooperFunc()
{
    _logWait.wait(1000);
    std::list<B1String> logs;
    {
        B1AutoLock al(_logLock);
        if (_logs.empty())
            return;
        logs.swap(_logs);
    }
    writeLogToFile(logs);
}

bool B1FileLog::start(B1String&& dirPath, B1String&& fileName, int32 maxRemainFileCount, B1String&& fileExtension)
{
    if (_started) {
        return true;
    }
    _fileExtension = std::move(fileExtension);
    B1SystemUtil::createDirectory(dirPath);
    if (B1SystemUtil::isFileExist(dirPath) != true) {
        return false;
    }
    bool result = B1Looper::run();
    if (result) {
        _dirPath = std::move(dirPath);
        _fileName = std::move(fileName);
        _maxRemainFileCount = maxRemainFileCount;
        _started = true;
    }
    return result;
}

void B1FileLog::stop()
{
    if (_started != true) {
        return;
    }

    _started = false;

    flush();
    for (int32 i = 0; i < 10; ++i) {
        bool flushed = false;
        {
            B1AutoLock al(_logLock);
            flushed = _logs.empty();
        }
        if (flushed) {
            break;
        }
        B1Thread::sleep(100);
    }
    B1Looper::quit();
    B1Looper::join();
    if (_fp) {
        fclose(_fp);
        _fp = NULL;
    }
    _lastDay = -1;
    _lastHour = -1;
    _maxRemainFileCount = -1;
    _fileName.clear();
    _dirPath.clear();
}

bool B1FileLog::testWriteLogFile()
{
    if (_dirPath.isEmpty()) {
        return false;
    }
    {   //  delete deprecated *.check files.
        std::list<B1String> deprecatedFiles;
        B1SystemUtil::findFiles(_dirPath, "check", &deprecatedFiles);
        for (const auto& deprecatedFile : deprecatedFiles) {
            B1SystemUtil::deleteFile(_dirPath + "/" + deprecatedFile);
        }
    }

    const B1Time current = B1Time::currentTime();
    int32 currentDay = current.day();
    int32 currentHour = current.hour();
    B1String filePath;
    filePath.format("%s/%s_%04d%02d%02d%02d.check",
                    _dirPath.cString(),
                    _fileName.cString(),
                    current.year(), current.month(), currentDay, currentHour);
    FILE* fp = fopen(filePath.cString(), "a");
    if (NULL == fp) {
        return false;
    }
    fprintf(fp, "write file checked at:%s", current.toString().cString());
    fclose(fp);
    return true;
}

bool B1FileLog::write(B1String&& log)
{
    if (_started != true) {
        return false;
    }

    int32 size = 0;
    {
        B1AutoLock al(_logLock);
        _logs.push_back(std::move(log));
        size = static_cast<int32>(_logs.size());
    }
    if (size > MAX_LOG_COUNT) {
        flush();
    }

    return true;
}

void B1FileLog::flush()
{
    _logWait.signal();
}

void B1FileLog::deleteDeprecatedFiles()
{
    std::list<B1String> deprecatedFiles;
    getDeprecatedFiles(&deprecatedFiles);
    for (const auto& deprecatedFile : deprecatedFiles) {
        B1SystemUtil::deleteFile(deprecatedFile);
    }
}
