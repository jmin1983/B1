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
    try {
        auto endPoint = _asioSocket->remote_endpoint();
        auto address = endPoint.address();
        return address.to_string();
    }
    catch (...) {
    }
    return "";
}

uint16 B1ASIOSocketImpl::implPeerPort() const
{
    try {
        auto endPoint = _asioSocket->remote_endpoint();
        return endPoint.port();
    }
    catch (...) {
    }
    return 0;
}

uint16 B1ASIOSocketImpl::implLocalPort() const
{
    try {
        return _asioSocket->local_endpoint().port();
    }
    catch (...) {
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
