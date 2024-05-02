//
// B1ArrayBufferReadWriteImpl.cpp
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Network.h"
#include "B1ArrayBufferReadWriteImpl.h"
#include "B1BaseSocket.h"
#include "B1BaseSocketImpl.h"

#include <B1Base/B1Lock.h>

#include <boost/bind/bind.hpp>

using namespace BnD;

B1ArrayBufferReadWriteImpl::B1ArrayBufferReadWriteImpl(B1ArrayBufferReadWriteImplListener* listener, size_t recvBufferSize)
    : B1BaseReadWriteImpl(listener)
    , _dataToWriteLock(new B1Lock())
    , _dataWriting(false)
    , _dataToWrite()
    , _recvBuffer(recvBufferSize, 0)
{
}

B1ArrayBufferReadWriteImpl::~B1ArrayBufferReadWriteImpl()
{
}

auto B1ArrayBufferReadWriteImpl::asioSocketImpl() const -> B1ASIOSocketImpl*
{
    return static_cast<B1ASIOSocketImpl*>(baseSocketImpl());
}

bool B1ArrayBufferReadWriteImpl::implRead()
{
    asioSocketImpl()->asioSocket()->async_read_some(boost::asio::buffer(_recvBuffer),
                                                    boost::bind(&B1ArrayBufferReadWriteImpl::readComplete, this,
                                                                boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    return true;
}

bool B1ArrayBufferReadWriteImpl::implOnReadComplete(size_t receivedBytes)
{
    if (auto l = listener()) {
        l->onReadComplete(&_recvBuffer[0], receivedBytes);
    }
    return true;    //  continue read();
}

void B1ArrayBufferReadWriteImpl::implOnWriteComplete(size_t transferredBytes)
{
    if (auto l = listener()) {
        l->onWriteComplete(transferredBytes);
    }
    std::vector<uint8> dataToSend;
    {
        B1AutoLock al(*_dataToWriteLock);
        if (_dataToWrite.empty()) {
            _dataWriting = false;
        }
        else {
            dataToSend.swap(_dataToWrite.front());
            _dataToWrite.pop_front();
        }
    }
    if (dataToSend.empty() != true) {
        writeData(&dataToSend[0], dataToSend.size());
    }
}

auto B1ArrayBufferReadWriteImpl::listener() const -> B1ArrayBufferReadWriteImplListener*
{
    return static_cast<B1ArrayBufferReadWriteImplListener*>(B1BaseReadWriteImpl::listener());
}

void B1ArrayBufferReadWriteImpl::writeData(const uint8* data, size_t size)
{
    boost::asio::async_write(*asioSocketImpl()->asioSocket(), boost::asio::buffer(data, size),
                             boost::bind(&B1ArrayBufferReadWriteImpl::writeComplete, this,
                                         boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void B1ArrayBufferReadWriteImpl::requestWriteData(const uint8* data, size_t size)
{
    if (size < 1) {
        assert(false);
        return;
    }
    bool writeImmediately = false;
    {
        B1AutoLock al(*_dataToWriteLock);
        if (_dataWriting) {
            _dataToWrite.push_back(std::vector<uint8>(data, data + size));
        }
        else {
            _dataWriting = true;
            writeImmediately = true;
        }
    }
    if (writeImmediately) {
        writeData(data, size);
    }
}
