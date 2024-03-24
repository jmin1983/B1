//
// B1Daemon.cpp
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Base.h"
#include "B1Daemon.h"

#ifdef _WIN32
#include "B1Daemon_windows.cpp"
#else 
#include "B1Daemon_linux.cpp"
#endif // _WIN32

#include <stdio.h>

using namespace BnD;

void B1Daemon::writeError(const char* file, const char* msg, int code)
{
    if (auto fp = fopen(file, "w")) {
        fprintf(fp, "[%d]%s\n", code, msg);
        fclose(fp);
    }
}
