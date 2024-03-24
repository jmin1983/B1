//
// B1BaseReadWriteImpl.cpp
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Network.h"
#include "B1BaseReadWriteImpl.h"
#include "B1BaseSocket.h"

using namespace BnD;

B1BaseReadWriteImpl::B1BaseReadWriteImpl(B1BaseSocket* baseSocket, B1BaseReadWriteImplListener* listener)
    : _baseSocket(baseSocket)
    , _listener(listener)
{
}

B1BaseReadWriteImpl::~B1BaseReadWriteImpl()
{
}

void B1BaseReadWriteImpl::readComplete(const boost::system::error_code& error, size_t receivedBytes)
{
    if (error) {
        B1LOG("read failed: reason[%d], message[%s]", error.value(), error.message().c_str());
        if (baseSocket()->isOpen()) {
            B1LOG("read failed but socket still opened -> close: peerAddress[%s], peerPort[%d], localPort[%d]",
                  baseSocket()->peerAddress().cString(), baseSocket()->peerPort(), baseSocket()->localPort());
            baseSocket()->close();
        }
        if (_listener) {
            _listener->onReadFailed(error.value());
        }
    }
    else {
        if (implOnReadComplete(receivedBytes)) {
            read(); //  continue reading.
        }
    }
}

void B1BaseReadWriteImpl::writeComplete(const boost::system::error_code& error, size_t transferredBytes)
{
    if (error) {
        B1LOG("write failed: reason[%d], transferredBytes:[%d], message[%s]", error.value(), transferredBytes, error.message().c_str());
        if (baseSocket()->isOpen()) {
            B1LOG("write failed but socket still opened -> close: peerAddress[%s], peerPort[%d], localPort[%d]",
                  baseSocket()->peerAddress().cString(), baseSocket()->peerPort(), baseSocket()->localPort());
            baseSocket()->close();
        }
        if (_listener) {
            _listener->onWriteFailed(error.value());
        }
    }
    else {
        implOnWriteComplete(transferredBytes);
    }
}

bool B1BaseReadWriteImpl::read()
{
    return implRead();
}
