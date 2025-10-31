//
// B1WebSocketClientSession.cpp
//
// Library: B1Http
// Package: Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Http.h"
#include "B1WebSocketClientSession.h"
#include "B1HttpMessage.h"
#include "B1HttpResponseGenerator.h"
#include "B1WebSocketImpl.h"

using namespace BnD;

B1WebSocketClientSession::B1WebSocketClientSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* listener)
    : B1BaseClientSession(clientSocket, listener)
{
}

B1WebSocketClientSession::~B1WebSocketClientSession()
{
}

void B1WebSocketClientSession::onReadFailed(int32 reason)
{
    setSessionStatusDisconnecting(reason);
}

void B1WebSocketClientSession::onWriteFailed(int32 reason)
{
    setSessionStatusDisconnecting(reason);
}

B1BaseReadWriteImpl* B1WebSocketClientSession::createReadWriteImpl()
{
    return new B1WebSocketReadWriteImpl(this);
}

void B1WebSocketClientSession::implOnConnect()
{
    B1BaseClientSession::implOnConnect();

    // Turn off the timeout on the tcp_stream, because the websocket stream has its own timeout system.
    boost::beast::get_lowest_layer(*readWriteImpl()->webSocketImpl()->webSocketStream()).expires_never();

    // Set suggested timeout settings for the websocket.
    readWriteImpl()->webSocketImpl()->webSocketStream()->set_option(
        boost::beast::websocket::stream_base::timeout::suggested(boost::beast::role_type::client));

    // Set a decorator to change the User-Agent of the handshake.
    readWriteImpl()->webSocketImpl()->webSocketStream()->set_option(boost::beast::websocket::stream_base::decorator(
        [](boost::beast::websocket::request_type& res) {
            res.set(boost::beast::http::field::user_agent, std::string(BOOST_BEAST_VERSION_STRING) + " websocket-client");
        }
    ));

    // Update the host_ string. This will provide the value of the Host HTTP header during the WebSocket handshake.
    // See https://tools.ietf.org/html/rfc7230#section-5.4
    try {
        readWriteImpl()->webSocketImpl()->webSocketStream()->handshake(B1String::formatAs("%s:%d", peerAddress().cString(), peerPort()).cString(), "/");
    }
    catch (std::exception const& e) {
        B1LOG("unable to handshake for websocket: %s", e.what());
        disconnect();
    }
}

B1WebSocketReadWriteImpl* B1WebSocketClientSession::readWriteImpl() const
{
    return static_cast<B1WebSocketReadWriteImpl*>(B1BaseClientSession::readWriteImpl());
}

void B1WebSocketClientSession::writeBinary(std::vector<uint8>&& data) const
{
    readWriteImpl()->addWriteBinary(std::move(data));
}

void B1WebSocketClientSession::writeText(B1String&& text) const
{
    readWriteImpl()->addWriteText(std::move(text));
}

void B1WebSocketClientSession::setTextBunchHint(uint32 value)
{
    readWriteImpl()->setTextBunchHint(value);
}
