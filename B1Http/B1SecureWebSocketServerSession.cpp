//
// B1SecureWebSocketServerSession.cpp
//
// Library: B1Http
// Package: Https
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Http.h"
#include "B1SecureWebSocketServerSession.h"
#include "B1HttpMessage.h"
#include "B1SecureWebSocketImpl.h"
#include "B1SecureWebSocketReadWriteImpl.h"

#include <boost/bind/bind.hpp>

using namespace BnD;

B1SecureWebSocketServerSession::B1SecureWebSocketServerSession(B1ServerSocket* serverSocket, B1BaseServerSessionListener* listener)
    : B1WebSocketServerSession(serverSocket, listener)
{
}

B1SecureWebSocketServerSession::~B1SecureWebSocketServerSession()
{
}

void B1SecureWebSocketServerSession::implAcceptWebSocket(const B1HttpServerMessage& initialMessage)
{
    // Turn off the timeout on the tcp_stream, because the websocket stream has its own timeout system.
    boost::beast::get_lowest_layer(*readWriteImpl()->secureWebSocketImpl()->secureWebSocketStream()).expires_never();

    // Set suggested timeout settings for the websocket
    readWriteImpl()->secureWebSocketImpl()->secureWebSocketStream()->set_option(
        boost::beast::websocket::stream_base::timeout::suggested(boost::beast::role_type::server));

    // Set a decorator to change the Server of the handshake.
    readWriteImpl()->secureWebSocketImpl()->secureWebSocketStream()->set_option(boost::beast::websocket::stream_base::decorator(
        [](boost::beast::websocket::response_type& res) {
            res.set(boost::beast::http::field::server, std::string(BOOST_BEAST_VERSION_STRING) + " secure-websocket-server");
        }
    ));

    // Accept the websocket handshake.
    readWriteImpl()->secureWebSocketImpl()->secureWebSocketStream()->async_accept(
        initialMessage.request(), boost::bind(&B1SecureWebSocketServerSession::acceptComplete, this, boost::asio::placeholders::error));
}

auto B1SecureWebSocketServerSession::createReadWriteImpl() -> B1BaseReadWriteImpl*
{
    return new B1WebSocketReadWriteImpl(this);
}

auto B1SecureWebSocketServerSession::readWriteImpl() const -> B1SecureWebSocketReadWriteImpl*
{
    return static_cast<B1SecureWebSocketReadWriteImpl*>(B1BaseServerSession::readWriteImpl());
}
