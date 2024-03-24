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

#include <boost/bind/bind.hpp>

using namespace BnD;

B1ArrayBufferReadWriteImpl::B1ArrayBufferReadWriteImpl(B1BaseSocket* baseSocket, B1ArrayBufferReadWriteImplListener* listener, size_t recvBufferSize)
    : B1BaseReadWriteImpl(baseSocket, listener)
    , _recvBuffer(recvBufferSize, 0)
{
}

B1ArrayBufferReadWriteImpl::~B1ArrayBufferReadWriteImpl()
{
}

bool B1ArrayBufferReadWriteImpl::implRead()
{
    baseSocket()->asioSocket()->async_read_some(boost::asio::buffer(_recvBuffer),
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
}

B1ArrayBufferReadWriteImplListener* B1ArrayBufferReadWriteImpl::listener() const
{
    return static_cast<B1ArrayBufferReadWriteImplListener*>(B1BaseReadWriteImpl::listener());
}

void B1ArrayBufferReadWriteImpl::writeData(const uint8* data, size_t size)
{
    boost::asio::async_write(*baseSocket()->asioSocket(), boost::asio::buffer(data, size),
                             boost::bind(&B1ArrayBufferReadWriteImpl::writeComplete, this,
                                         boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}
