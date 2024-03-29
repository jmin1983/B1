//
// B1BaseSocketImpl.cpp
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Network.h"
#include "B1BaseSocketImpl.h"
#include "B1IOContext.h"

using namespace BnD;

void B1ASIOSocketImpl::implUpdateSocket(std::shared_ptr<boost::asio::ip::tcp::socket>* pAsioSocket)
{
    _asioSocket = pAsioSocket->get();
}

auto B1ASIOSocketImpl::implRollbackSocket() -> std::shared_ptr<boost::asio::ip::tcp::socket>
{
    _asioSocket = NULL;
    return NULL;
}

void B1ASIOSocketImpl::implClose()
{
    if (_asioSocket && _asioSocket->is_open()) {
        try {
            _asioSocket->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        }
        catch (...) {}
        _asioSocket->close();
    }
}

B1String B1ASIOSocketImpl::implPeerAddress() const
{
    if (_asioSocket) {
        try {
            return _asioSocket->remote_endpoint().address().to_string();
        }
        catch (...) {
        }
    }
    return "";
}

uint16 B1ASIOSocketImpl::implPeerPort() const
{
    if (_asioSocket) {
        try {
            return _asioSocket->remote_endpoint().port();
        }
        catch (...) {
        }
    }
    return 0;
}

uint16 B1ASIOSocketImpl::implLocalPort() const
{
    if (_asioSocket) {
        try {
            return _asioSocket->local_endpoint().port();
        }
        catch (...) {
        }
    }
    return 0;
}

bool B1ASIOSocketImpl::implIsOpen() const
{
    try {
        return _asioSocket && _asioSocket->is_open();
    }
    catch (...) {
    }
    return false;
}

bool B1ASIOSocketImpl::implIsClosed() const
{
    return _asioSocket ? _asioSocket->is_open() != true : true;
}
