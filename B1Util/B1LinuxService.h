//
// B1LinuxService.h
//
// Library: B1Util
// Package: B1Util
// Module:  B1Util
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1UTIL_LINUX_SERVICE_H
#define _B1UTIL_LINUX_SERVICE_H

#if defined(__linux__)

namespace BnD {
    namespace B1LinuxService {
        bool startService(const B1String& serviceName);
        bool stopService(const B1String& serviceName);
        bool getServiceActive(const B1String& serviceName, bool* isActive);
    };
}

#endif  //  __linux__

#endif //   _B1UTIL_LINUX_SERVICE_H
