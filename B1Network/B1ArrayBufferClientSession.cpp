//
// B1ArrayBufferClientSession.cpp
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Network.h"
#include "B1ArrayBufferClientSession.h"

using namespace BnD;

B1ArrayBufferClientSession::B1ArrayBufferClientSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* listener)
    : B1ArrayBufferReadWriteImplListener()
    , B1BaseClientSession(clientSocket, listener)
{
}

B1ArrayBufferClientSession::~B1ArrayBufferClientSession()
{
}

void B1ArrayBufferClientSession::onReadFailed(int32 reason)
{
    setSessionStatusDisconnecting(reason);
}

void B1ArrayBufferClientSession::onWriteFailed(int32 reason)
{
    setSessionStatusDisconnecting(reason);
}

B1BaseReadWriteImpl* B1ArrayBufferClientSession::createReadWriteImpl()
{
    return new B1ArrayBufferReadWriteImpl(this, CONSTS_BUFFER_SIZE);
}

B1ArrayBufferReadWriteImpl* B1ArrayBufferClientSession::readWriteImpl() const
{
    return static_cast<B1ArrayBufferReadWriteImpl*>(B1BaseClientSession::readWriteImpl());
}

void B1ArrayBufferClientSession::writeData(const uint8* data, size_t size) const
{
    readWriteImpl()->requestWriteData(data, size);
}

bool B1ArrayBufferClientSession::writeData(const std::vector<uint8>& data) const
{
    auto dataSize = data.size();
    if (dataSize < 1) {
        return false;
    }
    writeData(&data[0], dataSize);
    return true;
}
