//
// B1WebSocketImpl.cpp
//
// Library: B1Http
// Package: Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Http.h"
#include "B1WebSocketImpl.h"

using namespace BnD;

B1WebSocketImpl::B1WebSocketImpl()
    : B1BaseSocketImpl()
    , _webSocketStream()
{
}

B1WebSocketImpl::~B1WebSocketImpl()
{
}

void B1WebSocketImpl::implUpdateSocket(std::shared_ptr<boost::asio::ip::tcp::socket>* pAsioSocket)
{
    auto asioSocket = pAsioSocket->get();
    if (_webSocketStream != NULL || asioSocket == NULL) {
        assert(false);
        return;
    }
    _webSocketStream = std::make_shared<boost::beast::websocket::stream<boost::beast::tcp_stream> >(std::move(*asioSocket));
    pAsioSocket->reset();
}

auto B1WebSocketImpl::implRollbackSocket() -> std::shared_ptr<boost::asio::ip::tcp::socket>
{
    if (_webSocketStream == NULL) {
        return NULL;
    }
    auto asioSocket = std::make_shared<boost::asio::ip::tcp::socket>(std::move(boost::beast::get_lowest_layer(*_webSocketStream).socket()));
    _webSocketStream.reset();
    return asioSocket;
}

void B1WebSocketImpl::implClose()
{
    if (_webSocketStream) {
        try {
            boost::beast::get_lowest_layer(*_webSocketStream).socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        }
        catch (...) {
        }
        try {
            boost::beast::get_lowest_layer(*_webSocketStream).socket().close();
        }
        catch (...) {
        }
    }
}

B1String B1WebSocketImpl::implPeerAddress() const
{
    if (_webSocketStream) {
        try {
            return boost::beast::get_lowest_layer(*_webSocketStream).socket().remote_endpoint().address().to_string();
        }
        catch (...) {
        }
    }
    return "";
}

uint16 B1WebSocketImpl::implPeerPort() const
{
    if (_webSocketStream) {
        try {
            return boost::beast::get_lowest_layer(*_webSocketStream).socket().remote_endpoint().port();
        }
        catch (...) {
        }
    }
    return 0;
}

uint16 B1WebSocketImpl::implLocalPort() const
{
    if (_webSocketStream) {
        try {
            return boost::beast::get_lowest_layer(*_webSocketStream).socket().local_endpoint().port();
        }
        catch (...) {
        }
    }
    return 0;
}

bool B1WebSocketImpl::implIsOpen() const
{
    //return _webSocketStream && _webSocketStream->is_open();   //  this means hand_shake is completed, not socket is_opened.
    return _webSocketStream && boost::beast::get_lowest_layer(*_webSocketStream).socket().is_open();
}

boost::asio::ip::tcp::socket* B1WebSocketImpl::implGetASIOSocket() const
{
    return _webSocketStream ? &boost::beast::get_lowest_layer(*_webSocketStream).socket() : NULL;
}
