//
// B1HttpsServerReadWriteImpl.cpp
//
// Library: B1Http
// Package: Https
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Http.h"
#include "B1HttpsServerReadWriteImpl.h"
#include "B1HttpMessage.h"
#include "B1SecureSocketImpl.h"

#include <boost/bind/bind.hpp>

using namespace BnD;

B1HttpsServerReadWriteImpl::B1HttpsServerReadWriteImpl(const B1SSLContext& sslContext, B1HttpServerReadWriteImplListener* listener)
    : B1HttpServerReadWriteImpl(listener)
    , _sslContext(sslContext)
{
}

void B1HttpsServerReadWriteImpl::implWriteResponse(boost::beast::http::message_generator&& response)
{
    bool keepAlive = response.keep_alive();
    boost::beast::async_write(*secureSocketImpl()->secureStream(), std::move(response),
                              boost::bind(&B1HttpsServerReadWriteImpl::writeResponseComplete, this,
                                          keepAlive, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

auto B1HttpsServerReadWriteImpl::createBaseSocketImpl() -> B1BaseSocketImpl*
{
    return new B1SecureSocketImpl(_sslContext);
}

bool B1HttpsServerReadWriteImpl::implRead()
{
    boost::beast::get_lowest_layer(*secureSocketImpl()->secureStream()).expires_after(std::chrono::seconds(30));
    secureSocketImpl()->secureStream()->async_handshake(boost::asio::ssl::stream_base::server, _httpMessage->buffer().data(),
                                                        boost::bind(&B1HttpsServerReadWriteImpl::handshakeComplete, this,
                                                                    boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    return true;
}

void B1HttpsServerReadWriteImpl::handshakeComplete(const boost::system::error_code& error, size_t bytesUsed)
{
    if (error) {
        B1LOG("handshake failed: reason[%d], message[%s]", error.value(), error.message().c_str());
        if (baseSocketImpl()->isOpen()) {
            B1LOG("handshake failed but socket still opened -> close: peerAddress[%s], peerPort[%d], localPort[%d]",
                  baseSocketImpl()->peerAddress().cString(), baseSocketImpl()->peerPort(), baseSocketImpl()->localPort());
            baseSocketImpl()->close();
        }
        if (auto l = listener()) {
            l->onReadFailed(error.value());
        }
    }
    else {
        _httpMessage->buffer().consume(bytesUsed);
        readAfterHandshake();
    }
}

void B1HttpsServerReadWriteImpl::readAfterHandshake()
{
    boost::beast::get_lowest_layer(*secureSocketImpl()->secureStream()).expires_after(std::chrono::seconds(30));
    _httpMessage->clearRequest();   // Make the request empty before reading, otherwise the operation behavior is undefined.
    boost::beast::http::async_read(*secureSocketImpl()->secureStream(), _httpMessage->buffer(), _httpMessage->request(),
                                   boost::bind(&B1HttpsServerReadWriteImpl::readComplete, this,
                                               boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

auto B1HttpsServerReadWriteImpl::secureSocketImpl() const -> B1SecureSocketImpl*
{
    return static_cast<B1SecureSocketImpl*>(baseSocketImpl());
}
