//
// B1WebSocketImpl.cpp
//
// Library: B1Http
// Package: B1Http
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
    , _peerAddress()
    , _peerPort(0)
    , _localPort(0)
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
    try {
        auto endPoint = asioSocket->remote_endpoint();
        _peerAddress = endPoint.address().to_string();
        _peerPort = endPoint.port();
        _localPort = asioSocket->local_endpoint().port();
    }
    catch (...) {}
    _webSocketStream.reset(new boost::beast::websocket::stream<boost::beast::tcp_stream>(std::move(*asioSocket)));
    pAsioSocket->reset();
}

void B1WebSocketImpl::implClose()
{
    boost::system::error_code ec;
    _webSocketStream->close(boost::beast::websocket::close_code::normal, ec);
}

B1String B1WebSocketImpl::implPeerAddress() const
{
    return _peerAddress.copy();
}

uint16 B1WebSocketImpl::implPeerPort() const
{
    return _peerPort;
}

uint16 B1WebSocketImpl::implLocalPort() const
{
    return _localPort;
}

bool B1WebSocketImpl::implIsOpen() const
{
    return _webSocketStream && _webSocketStream->is_open();
}
