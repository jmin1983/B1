//
// B1SecureWebSocketImpl.cpp
//
// Library: B1Http
// Package: Https
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Http.h"
#include "B1SecureWebSocketImpl.h"

#include <B1Network/B1SSLContext.h>

using namespace BnD;

B1SecureWebSocketImpl::B1SecureWebSocketImpl(const B1SSLContext& sslContext)
    : B1BaseSocketImpl()
    , _sslContext(sslContext)
    , _secureWebSocketStream()
{
}

B1SecureWebSocketImpl::~B1SecureWebSocketImpl()
{
}

void B1SecureWebSocketImpl::implUpdateSocket(std::shared_ptr<boost::asio::ip::tcp::socket>* pAsioSocket)
{
    auto asioSocket = pAsioSocket->get();
    if (_secureWebSocketStream != NULL || asioSocket == NULL) {
        assert(false);
        return;
    }
    _secureWebSocketStream.reset(new boost::beast::websocket::stream<boost::beast::ssl_stream<boost::beast::tcp_stream> >(std::move(*asioSocket), *_sslContext.nativeContext()));
    pAsioSocket->reset();
}

auto B1SecureWebSocketImpl::implRollbackSocket() -> std::shared_ptr<boost::asio::ip::tcp::socket>
{
    if (_secureWebSocketStream == NULL) {
        return std::shared_ptr<boost::asio::ip::tcp::socket>();
    }
    std::shared_ptr<boost::asio::ip::tcp::socket> asioSocket(new boost::asio::ip::tcp::socket(std::move(boost::beast::get_lowest_layer(*_secureWebSocketStream).socket())));
    _secureWebSocketStream.reset();
    return asioSocket;
}

void B1SecureWebSocketImpl::implClose()
{
    if (_secureWebSocketStream) {
        if (boost::beast::get_lowest_layer(*_secureWebSocketStream).socket().is_open()) {
            boost::beast::get_lowest_layer(*_secureWebSocketStream).socket().close();
        }
    }
}

B1String B1SecureWebSocketImpl::implPeerAddress() const
{
    if (_secureWebSocketStream) {
        try {
            return boost::beast::get_lowest_layer(*_secureWebSocketStream).socket().remote_endpoint().address().to_string();
        }
        catch (...) {
        }
    }
    return "";
}

uint16 B1SecureWebSocketImpl::implPeerPort() const
{
    if (_secureWebSocketStream) {
        try {
            return boost::beast::get_lowest_layer(*_secureWebSocketStream).socket().remote_endpoint().port();
        }
        catch (...) {
        }
    }
    return 0;
}

uint16 B1SecureWebSocketImpl::implLocalPort() const
{
    if (_secureWebSocketStream) {
        try {
            return boost::beast::get_lowest_layer(*_secureWebSocketStream).socket().local_endpoint().port();
        }
        catch (...) {
        }
    }
    return 0;
}

bool B1SecureWebSocketImpl::implIsOpen() const
{
    return _secureWebSocketStream && boost::beast::get_lowest_layer(*_secureWebSocketStream).socket().is_open();
}

bool B1SecureWebSocketImpl::implIsClosed() const
{
    return _secureWebSocketStream ? _secureWebSocketStream->is_open() != true : true;
}
