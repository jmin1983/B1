//
// B1HttpSocketImpl.cpp
//
// Library: B1Http
// Package: Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Http.h"
#include "B1HttpSocketImpl.h"

using namespace BnD;

B1HttpSocketImpl::B1HttpSocketImpl()
    : B1BaseSocketImpl()
    , _stream()
{
}

B1HttpSocketImpl::~B1HttpSocketImpl()
{
}

void B1HttpSocketImpl::implUpdateSocket(std::shared_ptr<boost::asio::ip::tcp::socket>* pAsioSocket)
{
    auto asioSocket = pAsioSocket->get();
    if (_stream != NULL || asioSocket == NULL) {
        assert(false);
        return;
    }
    _stream = std::make_shared<boost::beast::tcp_stream>(std::move(*asioSocket));
    pAsioSocket->reset();
}

auto B1HttpSocketImpl::implRollbackSocket() -> std::shared_ptr<boost::asio::ip::tcp::socket>
{
    if (_stream == NULL) {
        return NULL;
    }
    auto asioSocket = std::make_shared<boost::asio::ip::tcp::socket>(std::move(_stream->socket()));
    _stream.reset();
    return asioSocket;
}

void B1HttpSocketImpl::implClose()
{
    if (_stream) {
        try {
            _stream->socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        }
        catch (...) {
        }
        try {
            _stream->socket().close();
        }
        catch (...) {
        }
    }
}

B1String B1HttpSocketImpl::implPeerAddress() const
{
    if (_stream) {
        try {
            return _stream->socket().remote_endpoint().address().to_string();
        }
        catch (...) {
        }
    }
    return "";
}

uint16 B1HttpSocketImpl::implPeerPort() const
{
    if (_stream) {
        try {
            return _stream->socket().remote_endpoint().port();
        }
        catch (...) {
        }
    }
    return 0;
}

uint16 B1HttpSocketImpl::implLocalPort() const
{
    if (_stream) {
        try {
            return _stream->socket().local_endpoint().port();
        }
        catch (...) {
        }
    }
    return 0;
}

bool B1HttpSocketImpl::implIsOpen() const
{
    return _stream && _stream->socket().is_open();
}

boost::asio::ip::tcp::socket* B1HttpSocketImpl::implGetASIOSocket() const
{
    return _stream ? &_stream->socket() : NULL;
}
