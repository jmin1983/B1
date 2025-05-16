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

#include <sys/stat.h>

#ifdef _WIN32
#include "B1SystemUtil_windows.cpp"
#else 
#include "B1SystemUtil_linux.cpp"
#endif // _WIN32

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
