//
// B1MainServiceInterface.h
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1BASE_MAIN_SERVICE_INTERFACE_H
#define _B1BASE_MAIN_SERVICE_INTERFACE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif
 
namespace BnD {
    class B1MainServiceInterface {
    public:
        virtual bool start() = 0;
        virtual void wait() = 0;
        virtual void stop() = 0;
        virtual bool isStopped() const = 0;
    };
}   //  !BnD

#endif  // !_B1BASE_MAIN_SERVICE_INTERFACE_H
