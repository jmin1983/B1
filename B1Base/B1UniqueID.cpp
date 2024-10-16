//
// B1UniqueID.cpp
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Base.h"
#include "B1UniqueID.h"
#include "B1SystemUtil.h"

using namespace BnD;

B1UniqueID::B1UniqueID()
    : _fp(NULL)
    , _lock()
    , _lastID(-1)
    , _range()
{
}

B1UniqueID::~B1UniqueID()
{
    finalize();
}

bool B1UniqueID::writeLastIDToFile()
{
    if (NULL == _fp) {
        return false;
    }
    fseek(_fp, 0, SEEK_SET);
    if (1 != fwrite(&_lastID, sizeof(_lastID), 1, _fp)) {
        return false;
    }
    fflush(_fp);
    return true;
}

bool B1UniqueID::initialize(int64 begin, int64 end, const B1String& dirPath, const B1String& fileName)
{
    if (_fp != NULL) {
        return false;
    }
    if (begin > end) {
        return false;
    }
    _range.first = begin;
    _range.second = end;
    const B1String fileFullPath = dirPath + "/" + fileName;
    if (B1SystemUtil::isFileExist(fileFullPath)) {
        FILE* fp = fopen(fileFullPath.cString(), "rb");
        if (NULL == fp) {
            return false;
        }
        if (fread(&_lastID, sizeof(_lastID), 1, fp) == 0) {
            _lastID = -1;
        }
        fclose(fp);
    }
    else {
        B1SystemUtil::createDirectory(dirPath);
    }
    _fp = fopen(fileFullPath.cString(), "wb");
    if (NULL == _fp) {
        return false;
    }
    if (_lastID < begin || _lastID >= end) {
        _lastID = begin;
    }
    writeLastIDToFile();
    return true;
}

void B1UniqueID::finalize()
{
    if (_fp) {
        fclose(_fp);
        _fp = NULL;
    }
}

int64 B1UniqueID::generateNextID()
{
    int64 nextID = -1;
    {
        B1AutoLock al(_lock);
        if (++_lastID >= _range.second) {
            _lastID = _range.first;
        }
        if (writeLastIDToFile() != true) {
            return -1;
        }
        nextID = _lastID;
    }
    return nextID;
}

bool B1UniqueID::isInRange(int64 id) const
{
    return _range.first <= id && id < _range.second;
}
