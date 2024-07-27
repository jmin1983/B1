//
// B1ArrayBufferReadWriteImpl.h
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1NETWORK_ARRAY_BUFFER_READ_WRITE_IMPL_H
#define _B1NETWORK_ARRAY_BUFFER_READ_WRITE_IMPL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1BaseReadWriteImpl.h>

#include <list>

namespace BnD {
    class B1Lock;
    class B1ArrayBufferReadWriteImplListener : public B1BaseReadWriteImplListener {
    public:
        virtual void onReadComplete(uint8* data, size_t dataSize) {}
        virtual void onWriteComplete(size_t dataSize) {}
    };

    class B1ArrayBufferReadWriteImpl : public B1BaseReadWriteImpl {
    public:
        B1ArrayBufferReadWriteImpl(B1ArrayBufferReadWriteImplListener* listener, size_t recvBufferSize);
        virtual ~B1ArrayBufferReadWriteImpl();
    private:
        std::shared_ptr<B1Lock> _dataToWriteLock;
        bool _dataWriting;
        std::list<std::vector<uint8> > _dataToWrite;
    protected:
        std::vector<uint8> _recvBuffer;
    private:
        class B1ASIOSocketImpl* asioSocketImpl() const;
    protected:
        bool implRead() final;
        bool implOnReadComplete(size_t receivedBytes) final;    //  return false if there are no more data to read.
        void implOnWriteFailed(int32 reason) final;
        void implOnWriteComplete(size_t transferredBytes) final;
    protected:
        B1ArrayBufferReadWriteImplListener* listener() const;
        void writeData(const uint8* data, size_t size);
    public:
        void requestWriteData(const uint8* data, size_t size);
    };
}   //  !BnD

#endif  // !_B1NETWORK_ARRAY_BUFFER_READ_WRITE_IMPL_H
