//
// B1IOLooper.h
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1NETWORK_IO_LOOPER_H
#define _B1NETWORK_IO_LOOPER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Base/B1Looper.h>

namespace BnD {
    class B1IOContext;
    class B1IOLooper : protected B1Looper {
    public:
        B1IOLooper();
        virtual ~B1IOLooper();
    protected:
        enum CONSTS {
            CONSTS_THREAD_COUNT_HINT = 1,
        };
    protected:
        std::shared_ptr<B1IOContext> _ioContext;
    protected:
        virtual void implLooperFunc() override;
    protected:
        virtual bool implStartup() { return true; }
        virtual void implShutdown() {}
    public:
        bool startup();
        void shutdown();
        B1IOContext* ioContext() { return _ioContext.get(); }
        bool pause();
        bool resume();
    };
}   //  !BnD

#endif  // !_B1NETWORK_IO_LOOPER_H
