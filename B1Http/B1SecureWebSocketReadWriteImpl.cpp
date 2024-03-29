//
// B1SecureWebSocketReadWriteImpl.cpp
//
// Library: B1Http
// Package: https
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Http.h"
#include "B1SecureWebSocketReadWriteImpl.h"
#include "B1SecureWebSocketImpl.h"
#include "B1WebSocketMessage.h"

#include <boost/bind/bind.hpp>

using namespace BnD;

B1SecureWebSocketReadWriteImpl::B1SecureWebSocketReadWriteImpl(const B1SSLContext& sslContext, B1WebSocketReadWriteImplListener* listener)
    : B1WebSocketReadWriteImpl(listener)
    , _sslContext(sslContext)
{
}

B1SecureWebSocketReadWriteImpl::~B1SecureWebSocketReadWriteImpl()
{
}

void B1SecureWebSocketReadWriteImpl::implWriteBinary(std::vector<uint8>&& data)
{
    secureWebSocketImpl()->secureWebSocketStream()->binary(true);

    auto temp = new std::vector<uint8>(std::move(data));
    secureWebSocketImpl()->secureWebSocketStream()->async_write(boost::beast::net::buffer(*temp),
                                                                boost::bind(&B1SecureWebSocketReadWriteImpl::writeBinaryComplete, this, temp,
                                                                            boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void B1SecureWebSocketReadWriteImpl::implWriteText(B1String&& data)
{
    secureWebSocketImpl()->secureWebSocketStream()->text(true);

    auto temp = new std::string(std::move(const_cast<std::string&>(data.to_string())));
    secureWebSocketImpl()->secureWebSocketStream()->async_write(boost::beast::net::buffer(*temp),
                                                                boost::bind(&B1SecureWebSocketReadWriteImpl::writeTextComplete, this,
                                                                            temp, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

B1BaseSocketImpl* B1SecureWebSocketReadWriteImpl::createBaseSocketImpl()
{
    return new B1SecureWebSocketImpl(_sslContext);
}

bool B1SecureWebSocketReadWriteImpl::implRead()
{
    try {
        secureWebSocketImpl()->secureWebSocketStream()->async_read(_webSocketMessage->buffer(),
                                                                   boost::bind(&B1SecureWebSocketReadWriteImpl::readComplete, this,
                                                                               boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
    catch (...) {
        return false;
    }
    return true;
}

bool B1SecureWebSocketReadWriteImpl::implOnReadComplete(size_t receivedBytes)
{
    bool continueReading = false;
    boost::ignore_unused(receivedBytes);
    if (auto l = listener()) {
        if (secureWebSocketImpl()->secureWebSocketStream()->got_text()) {
            continueReading = l->onWebSocketReadComplete(boost::beast::buffers_to_string(_webSocketMessage->buffer().data()));
        }
        else {
            auto bufferData = _webSocketMessage->buffer().data();
            std::vector<uint8> buffer((uint8*)bufferData.data(), (uint8*)bufferData.data() + bufferData.size());
            continueReading = l->onWebSocketReadComplete(std::move(buffer));
        }
        _webSocketMessage->clearRequest();
    }
    return continueReading;
}

auto B1SecureWebSocketReadWriteImpl::secureWebSocketImpl() const -> B1SecureWebSocketImpl*
{
    return static_cast<B1SecureWebSocketImpl*>(baseSocketImpl());
}
