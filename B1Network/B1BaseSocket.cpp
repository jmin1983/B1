//
// B1BaseSocket.cpp
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Network.h"
#include "B1BaseSocket.h"
#include "B1BaseSocketImpl.h"
#include "B1IOContext.h"

using namespace BnD;

B1BaseSocket::B1BaseSocket()
    : _impl(NULL)
    , _asioSocket()
{
}

B1BaseSocket::~B1BaseSocket()
{
}

void B1BaseSocket::resetSocket(const B1IOContext* context)
{
    try {
        if (context) {
            _asioSocket.reset(new boost::asio::ip::tcp::socket(*context->nativeContext()));
        }
        else {
            _asioSocket.reset();
        }
    }
    catch (...) {
        _asioSocket.reset();
    }
}

void B1BaseSocket::close()
{
    if (_impl) {
        _impl->close();
    }
    else {
        if (_asioSocket) {
            try {
                _asioSocket->close();
            }
            catch (...) {
            }
        }
    }
}

B1String B1BaseSocket::peerAddress() const
{
    return _impl ? _impl->peerAddress() : "";
}

uint16 B1BaseSocket::peerPort() const
{
    return _impl ? _impl->peerPort() : 0;
}

uint16 B1BaseSocket::localPort() const
{
    return _impl ? _impl->localPort() : 0;
}

bool B1BaseSocket::isOpen() const
{
    return _impl ? _impl->isOpen() : false;
}

void B1BaseSocket::setImpl(B1BaseSocketImpl* impl)
{
    _impl = impl;
    if (_impl) {
        _impl->updateSocket(&_asioSocket);
    }
}

void B1BaseSocket::resetImpl()
{
    if (_impl) {
        if (auto asioSocket = _impl->rollbackSocket()) {
            _asioSocket = asioSocket;
        }
        _impl = NULL;
    }
}

void B1BaseSocket::cleanUp()
{
    if (_asioSocket && _asioSocket->is_open()) {
        B1LOG("asio still open! -> close");
        try {
            _asioSocket->close();
        }
        catch (...) {
        }
    }
    _asioSocket.reset();
}
