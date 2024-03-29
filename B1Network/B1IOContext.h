//
// B1IOContext.h
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1NETWORK_IO_CONTEXT_H
#define _B1NETWORK_IO_CONTEXT_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace boost {
    namespace asio {
        class io_context;
    }
}

namespace BnD {
    class B1IOContext {
    public:
        B1IOContext(uint8 threadCountHint);
        ~B1IOContext();
    private:
        boost::asio::io_context* _context;
    public:
        void stop();
        bool isStopped() const;
        boost::asio::io_context* nativeContext() const { return _context; }
        void runNativeContext();
        void stopNativeContext();
        void restartNativeContext();
        bool isNativeContextStopped();
    };
}   //  !BnD

#endif  // !_B1NETWORK_IO_CONTEXT_H
