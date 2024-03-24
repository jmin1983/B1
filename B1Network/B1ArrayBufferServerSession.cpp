//
// B1ArrayBufferServerSession.cpp
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Network.h"
#include "B1ArrayBufferServerSession.h"

using namespace BnD;

B1ArrayBufferServerSession::B1ArrayBufferServerSession(B1ServerSocket* serverSocket, B1BaseServerSessionListener* listener)
    : B1ArrayBufferReadWriteImplListener()
    , B1BaseServerSession(serverSocket, listener)
{
}

B1ArrayBufferServerSession::~B1ArrayBufferServerSession()
{
}

void B1ArrayBufferServerSession::onReadFailed(int32 reason)
{
    setSessionStatusDisconnected(reason);
}

void B1ArrayBufferServerSession::onWriteFailed(int32 reason)
{
    setSessionStatusDisconnected(reason);
}

B1BaseReadWriteImpl* B1ArrayBufferServerSession::createReadWriteImpl(B1BaseSocket* baseSocket)
{
    return new B1ArrayBufferReadWriteImpl(baseSocket, this, CONSTS_BUFFER_SIZE);
}

B1ArrayBufferReadWriteImpl* B1ArrayBufferServerSession::readWriteImpl() const
{
    return static_cast<B1ArrayBufferReadWriteImpl*>(B1BaseServerSession::readWriteImpl());
}

void B1ArrayBufferServerSession::writeData(const uint8* data, size_t size) const
{
    readWriteImpl()->writeData(data, size);
}

bool B1ArrayBufferServerSession::writeData(const std::vector<uint8>& data) const
{
    auto dataSize = data.size();
    if (dataSize < 1) {
        return false;
    }
    writeData(&data[0], dataSize);
    return true;
}
