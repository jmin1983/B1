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
    class B1BaseSocketImpl;
    class B1BaseReadWriteImplListener {
    public:
        virtual void onReadFailed(int32 reason) {}
        virtual void onWriteFailed(int32 reason) {}
    };

    class B1BaseReadWriteImpl {
    public:
        B1BaseReadWriteImpl(B1BaseReadWriteImplListener* listener);
        virtual ~B1BaseReadWriteImpl();
    private:
        std::shared_ptr<B1BaseSocketImpl> _baseSocketImpl;
        B1BaseReadWriteImplListener* _listener;
    protected:
        virtual B1BaseSocketImpl* createBaseSocketImpl();
        virtual bool implRead() = 0;
        virtual bool implOnReadComplete(size_t receivedBytes) = 0;  //  return false if there are no more data to read.
        virtual void implOnWriteFailed(int32 reason) {}
        virtual void implOnWriteComplete(size_t transferredBytes) {}
    protected:
        B1BaseReadWriteImplListener* listener() const { return _listener; }
        void readComplete(const boost::system::error_code& error, size_t receivedBytes);
        void writeComplete(const boost::system::error_code& error, size_t transferredBytes);
    public:
        bool initialize();
        void finalize();
        bool read();
        void close();
        B1String peerAddress() const;
        uint16 peerPort() const;
        uint16 localPort() const;
        bool isOpen() const;
        B1BaseSocketImpl* baseSocketImpl() const { return _baseSocketImpl.get(); }
    };
}   //  !BnD

#endif  // !_B1NETWORK_BASE_READ_WRITE_IMPL_H
