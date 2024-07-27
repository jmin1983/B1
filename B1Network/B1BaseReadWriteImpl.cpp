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
#include "B1BaseSocketImpl.h"

using namespace BnD;

B1BaseReadWriteImpl::B1BaseReadWriteImpl(B1BaseReadWriteImplListener* listener)
    : _baseSocketImpl()
    , _listener(listener)
{
}

B1BaseReadWriteImpl::~B1BaseReadWriteImpl()
{
}

auto B1BaseReadWriteImpl::createBaseSocketImpl() -> B1BaseSocketImpl*
{
    return new B1ASIOSocketImpl();
}

void B1BaseReadWriteImpl::readComplete(const boost::system::error_code& error, size_t receivedBytes)
{
    if (error) {
        B1LOG("read failed: reason[%d], message[%s]", error.value(), error.message().c_str());
        if (baseSocketImpl()->isOpen()) {
            B1LOG("read failed but socket still opened -> close: peerAddress[%s], peerPort[%d], localPort[%d]",
                  baseSocketImpl()->peerAddress().cString(), baseSocketImpl()->peerPort(), baseSocketImpl()->localPort());
            baseSocketImpl()->close();
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
        if (baseSocketImpl()->isOpen()) {
            B1LOG("write failed but socket still opened -> close: peerAddress[%s], peerPort[%d], localPort[%d]",
                baseSocketImpl()->peerAddress().cString(), baseSocketImpl()->peerPort(), baseSocketImpl()->localPort());
            baseSocketImpl()->close();
        }
        implOnWriteFailed(error.value());
        if (_listener) {
            _listener->onWriteFailed(error.value());
        }
    }
    else {
        implOnWriteComplete(transferredBytes);
    }
}

bool B1BaseReadWriteImpl::initialize()
{
    _baseSocketImpl.reset(createBaseSocketImpl());
    return true;
}

void B1BaseReadWriteImpl::finalize()
{
    if (_baseSocketImpl) {
        assert(_baseSocketImpl->isOpen() != true);
        _baseSocketImpl.reset();
    }
}

bool B1BaseReadWriteImpl::read()
{
    return implRead();
}

void B1BaseReadWriteImpl::close()
{
    if (_baseSocketImpl) {
        _baseSocketImpl->close();
    }
}

B1String B1BaseReadWriteImpl::peerAddress() const
{
    return _baseSocketImpl ? _baseSocketImpl->peerAddress() : "";
}

uint16 B1BaseReadWriteImpl::peerPort() const
{
    return _baseSocketImpl ? _baseSocketImpl->peerPort() : 0;
}

uint16 B1BaseReadWriteImpl::localPort() const
{
    return _baseSocketImpl ? _baseSocketImpl->localPort() : 0;
}

bool B1BaseReadWriteImpl::isOpen() const
{
    return _baseSocketImpl ? _baseSocketImpl->isOpen() : false;
}

bool B1BaseReadWriteImpl::isClosed() const
{
    return _baseSocketImpl ? _baseSocketImpl->isClosed() : true;
}
