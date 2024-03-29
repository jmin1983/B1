//
// B1HttpsReadWriteImpl.cpp
//
// Library: B1Http
// Package: Https
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Http.h"
#include "B1HttpsReadWriteImpl.h"
#include "B1HttpMessage.h"
#include "B1SecureSocketImpl.h"

#include <boost/bind/bind.hpp>

using namespace BnD;

B1HttpsReadWriteImpl::B1HttpsReadWriteImpl(const B1SSLContext& sslContext, B1HttpReadWriteImplListener* listener)
    : B1HttpReadWriteImpl(listener)
    , _sslContext(sslContext)
{
}

void B1HttpsReadWriteImpl::implWriteResponse(boost::beast::http::message_generator&& response)
{
    bool keepAlive = response.keep_alive();
    boost::beast::async_write(*secureSocketImpl()->secureStream(), std::move(response),
                              boost::bind(&B1HttpsReadWriteImpl::writeResponseComplete, this,
                                          keepAlive, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

auto B1HttpsReadWriteImpl::createBaseSocketImpl() -> B1BaseSocketImpl*
{
    return new B1SecureSocketImpl(_sslContext);
}

bool B1HttpsReadWriteImpl::implRead()
{
    boost::beast::get_lowest_layer(*secureSocketImpl()->secureStream()).expires_after(std::chrono::seconds(30));
    secureSocketImpl()->secureStream()->async_handshake(boost::asio::ssl::stream_base::server, _httpMessage->buffer().data(),
                                                        boost::bind(&B1HttpsReadWriteImpl::handshakeComplete, this,
                                                                    boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    return true;
}

void B1HttpsReadWriteImpl::handshakeComplete(const boost::system::error_code& error, size_t bytesUsed)
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

void B1HttpsReadWriteImpl::readAfterHandshake()
{
    boost::beast::get_lowest_layer(*secureSocketImpl()->secureStream()).expires_after(std::chrono::seconds(30));
    _httpMessage->clearRequest();   // Make the request empty before reading, otherwise the operation behavior is undefined.
    boost::beast::http::async_read(*secureSocketImpl()->secureStream(), _httpMessage->buffer(), _httpMessage->request(),
                                   boost::bind(&B1HttpsReadWriteImpl::readComplete, this,
                                               boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

auto B1HttpsReadWriteImpl::secureSocketImpl() const -> B1SecureSocketImpl*
{
    return static_cast<B1SecureSocketImpl*>(baseSocketImpl());
}
