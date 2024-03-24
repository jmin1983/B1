//
// B1BaseReadWriteImpl.h
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1NETWORK_BASE_READ_WRITE_IMPL_H
#define _B1NETWORK_BASE_READ_WRITE_IMPL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace boost {
    namespace system {
        class error_code;
    };
};

namespace BnD {
    class B1BaseSocket;
    class B1BaseReadWriteImplListener {
    public:
        virtual void onReadFailed(int32 reason) {}
        virtual void onWriteFailed(int32 reason) {}
    };

    class B1BaseReadWriteImpl {
    public:
        B1BaseReadWriteImpl(B1BaseSocket* baseSocket, B1BaseReadWriteImplListener* listener);
        virtual ~B1BaseReadWriteImpl();
    private:
        B1BaseSocket* _baseSocket;
        B1BaseReadWriteImplListener* _listener;
    protected:
        virtual bool implRead() = 0;
        virtual bool implOnReadComplete(size_t receivedBytes) = 0;  //  return false if there are no more data to read.
        virtual void implOnWriteComplete(size_t transferredBytes) {}
    protected:
        B1BaseSocket* baseSocket() const { return _baseSocket; }
        B1BaseReadWriteImplListener* listener() const { return _listener; }
        void readComplete(const boost::system::error_code& error, size_t receivedBytes);
        void writeComplete(const boost::system::error_code& error, size_t transferredBytes);
    public:
        bool read();
    };
}   //  !BnD

#endif  // !_B1NETWORK_BASE_READ_WRITE_IMPL_H
