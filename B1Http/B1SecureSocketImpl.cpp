//
// B1SecureSocketImpl.cpp
//
// Library: B1Http
// Package: Https
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Http.h"
#include "B1SecureSocketImpl.h"

#include <B1Network/B1SSLContext.h>

#include <boost/bind/bind.hpp>

using namespace BnD;

B1SecureSocketImpl::B1SecureSocketImpl(const B1SSLContext& sslContext)
    : B1BaseSocketImpl()
    , _sslContext(sslContext)
    , _secureStream()
{
}

B1SecureSocketImpl::~B1SecureSocketImpl()
{
}

void B1SecureSocketImpl::implUpdateSocket(std::shared_ptr<boost::asio::ip::tcp::socket>* pAsioSocket)
{
    auto asioSocket = pAsioSocket->get();
    if (_secureStream != NULL || asioSocket == NULL) {
        assert(false);
        return;
    }
    _secureStream = std::make_shared<boost::beast::ssl_stream<boost::beast::tcp_stream> >(std::move(*asioSocket), *_sslContext.nativeContext());
    pAsioSocket->reset();
}

auto B1SecureSocketImpl::implRollbackSocket() -> std::shared_ptr<boost::asio::ip::tcp::socket>
{
    if (_secureStream == NULL) {
        return std::shared_ptr<boost::asio::ip::tcp::socket>();
    }
    auto asioSocket = std::make_shared<boost::asio::ip::tcp::socket>(std::move(boost::beast::get_lowest_layer(*_secureStream).socket()));
    _secureStream.reset();
    return asioSocket;
}

void B1SecureSocketImpl::implClose()
{
    if (_secureStream) {
        try {
            boost::beast::get_lowest_layer(*_secureStream).socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        }
        catch (...) {
        }
        try {
            boost::beast::get_lowest_layer(*_secureStream).socket().close();
        }
        catch (...) {
        }
    }
}

B1String B1SecureSocketImpl::implPeerAddress() const
{
    if (_secureStream) {
        try {
            return boost::beast::get_lowest_layer(*_secureStream).socket().remote_endpoint().address().to_string();
        }
        catch (...) {
        }
    }
    return "";
}

uint16 B1SecureSocketImpl::implPeerPort() const
{
    if (_secureStream) {
        try {
            return boost::beast::get_lowest_layer(*_secureStream).socket().remote_endpoint().port();
        }
        catch (...) {
        }
    }
    return 0;
}

uint16 B1SecureSocketImpl::implLocalPort() const
{
    if (_secureStream) {
        try {
            return boost::beast::get_lowest_layer(*_secureStream).socket().local_endpoint().port();
        }
        catch (...) {
        }
    }
    return 0;
}

bool B1SecureSocketImpl::implIsOpen() const
{
    return _secureStream && boost::beast::get_lowest_layer(*_secureStream).socket().is_open();
}

boost::asio::ip::tcp::socket* B1SecureSocketImpl::implGetASIOSocket() const
{
    return _secureStream ? &boost::beast::get_lowest_layer(*_secureStream).socket() : NULL;
}
