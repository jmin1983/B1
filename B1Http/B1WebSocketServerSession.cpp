//
// B1WebSocketServerSession.cpp
//
// Library: B1Http
// Package: B1Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Http.h"
#include "B1WebSocketServerSession.h"
#include "B1HttpMessage.h"
#include "B1HttpResponseGenerator.h"
#include "B1WebSocketImpl.h"

#include <B1Network/B1ServerSocket.h>

#include <boost/bind/bind.hpp>

using namespace BnD;

B1WebSocketServerSession::B1WebSocketServerSession(B1ServerSocket* serverSocket, B1BaseServerSessionListener* listener)
    : B1BaseServerSession(serverSocket, listener)
{
}

B1WebSocketServerSession::~B1WebSocketServerSession()
{
}

void B1WebSocketServerSession::onReadFailed(int32 reason)
{
    setSessionStatusDisconnected(reason);
}

void B1WebSocketServerSession::onWriteFailed(int32 reason)
{
    setSessionStatusDisconnected(reason);
}

B1BaseSocketImpl* B1WebSocketServerSession::createBaseSocketImpl()
{
    return new B1WebSocketImpl();
}

B1BaseReadWriteImpl* B1WebSocketServerSession::createReadWriteImpl(B1BaseSocket* baseSocket)
{
    return new B1WebSocketReadWriteImpl(baseSocket, this);
}

void B1WebSocketServerSession::acceptComplete(const boost::system::error_code& error)
{
    if (error) {
        B1LOG("accept error: peerAddress[%s], localPort[%d], reason[%d]", serverSocket()->peerAddress().cString(), serverSocket()->localPort(), error.value());
        disconnect();
    }
    else {
        onWebSocketAccepted();
        beginRead();
    }
}

B1WebSocketReadWriteImpl* B1WebSocketServerSession::readWriteImpl() const
{
    return static_cast<B1WebSocketReadWriteImpl*>(B1BaseServerSession::readWriteImpl());
}

B1WebSocketImpl* B1WebSocketServerSession::webSocketImpl() const
{
    return static_cast<B1WebSocketImpl*>(_baseSocketImpl.get());
}

void B1WebSocketServerSession::writeBinary(std::vector<uint8>&& data) const
{
    readWriteImpl()->addWriteBinary(std::move(data));
}

void B1WebSocketServerSession::writeText(B1String&& text) const
{
    readWriteImpl()->addWriteText(std::move(text));
}

void B1WebSocketServerSession::acceptWebSocket(const B1HttpMessage& initialMessage)
{
    readWriteImpl()->setWebSocketImpl(webSocketImpl());
    webSocketImpl()->webSocketStream()->set_option(boost::beast::websocket::stream_base::timeout::suggested(boost::beast::role_type::server));

    // Set a decorator to change the Server of the handshake.
    webSocketImpl()->webSocketStream()->set_option(boost::beast::websocket::stream_base::decorator(
        [](boost::beast::websocket::response_type& res) { res.set(boost::beast::http::field::server, std::string(BOOST_BEAST_VERSION_STRING) + " websocket-server"); }
    ));

    // Accept the websocket handshake.
    webSocketImpl()->webSocketStream()->async_accept(initialMessage.request(),
                                                     boost::bind(&B1WebSocketServerSession::acceptComplete, this, boost::asio::placeholders::error));
}
