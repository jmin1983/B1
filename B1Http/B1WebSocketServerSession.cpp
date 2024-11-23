//
// B1WebSocketServerSession.cpp
//
// Library: B1Http
// Package: Http
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

void B1WebSocketServerSession::implAcceptWebSocket(const B1HttpMessage& initialMessage)
{
    // Turn off the timeout on the tcp_stream, because the websocket stream has its own timeout system.
    boost::beast::get_lowest_layer(*readWriteImpl()->webSocketImpl()->webSocketStream()).expires_never();

    // Set suggested timeout settings for the websocket
    readWriteImpl()->webSocketImpl()->webSocketStream()->set_option(
        boost::beast::websocket::stream_base::timeout::suggested(boost::beast::role_type::server));

    // Set a decorator to change the Server of the handshake.
    readWriteImpl()->webSocketImpl()->webSocketStream()->set_option(boost::beast::websocket::stream_base::decorator(
        [](boost::beast::websocket::response_type& res) {
            res.set(boost::beast::http::field::server, std::string(BOOST_BEAST_VERSION_STRING) + " websocket-server");
        }
    ));

    // Accept the websocket handshake.
    readWriteImpl()->webSocketImpl()->webSocketStream()->async_accept(
        initialMessage.request(), boost::bind(&B1WebSocketServerSession::acceptComplete, this, boost::asio::placeholders::error));
}

void B1WebSocketServerSession::onReadFailed(int32 reason)
{
    setSessionStatusDisconnecting(reason);
}

void B1WebSocketServerSession::onWriteFailed(int32 reason)
{
    setSessionStatusDisconnecting(reason);
}

B1BaseReadWriteImpl* B1WebSocketServerSession::createReadWriteImpl()
{
    return new B1WebSocketReadWriteImpl(this);
}

B1WebSocketReadWriteImpl* B1WebSocketServerSession::readWriteImpl() const
{
    return static_cast<B1WebSocketReadWriteImpl*>(B1BaseServerSession::readWriteImpl());
}

void B1WebSocketServerSession::acceptComplete(const boost::system::error_code& error)
{
    if (error) {
        B1LOG("accept error: peerAddress[%s], localPort[%d], reason[%d]",
            B1BaseServerSession::readWriteImpl()->peerAddress().cString(), B1BaseServerSession::readWriteImpl()->localPort(), error.value());
        disconnect();
    }
    else {
        onWebSocketAccepted();
        beginRead();
    }
}

void B1WebSocketServerSession::acceptWebSocket(const B1HttpMessage& initialMessage)
{
    implAcceptWebSocket(initialMessage);
}

void B1WebSocketServerSession::writeBinary(std::vector<uint8>&& data) const
{
    readWriteImpl()->addWriteBinary(std::move(data));
}

void B1WebSocketServerSession::writeText(B1String&& text) const
{
    readWriteImpl()->addWriteText(std::move(text));
}

void B1WebSocketServerSession::setTextBunchHint(uint32 value)
{
    readWriteImpl()->setTextBunchHint(value);
}
