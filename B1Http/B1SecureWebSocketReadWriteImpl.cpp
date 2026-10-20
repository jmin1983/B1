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

    auto temp = std::make_shared<std::vector<uint8> >(std::move(data));
    secureWebSocketImpl()->secureWebSocketStream()->async_write(
        boost::beast::net::buffer(*temp),
        [this, temp](const boost::system::error_code& error, size_t transferredBytes) {
            this->writeBinaryComplete(temp, error, transferredBytes);
        }
    );
}

void B1SecureWebSocketReadWriteImpl::implWriteText(B1String&& data)
{
    secureWebSocketImpl()->secureWebSocketStream()->text(true);

    auto temp = std::make_shared<std::string>(std::move(const_cast<std::string&>(data.to_string())));
    secureWebSocketImpl()->secureWebSocketStream()->async_write(
        boost::beast::net::buffer(*temp),
        [this, temp](const boost::system::error_code& error, size_t transferredBytes) {
            this->writeTextComplete(temp, error, transferredBytes);
        }
    );
}

B1BaseSocketImpl* B1SecureWebSocketReadWriteImpl::createBaseSocketImpl()
{
    return new B1SecureWebSocketImpl(_sslContext);
}

bool B1SecureWebSocketReadWriteImpl::implRead()
{
    try {
        secureWebSocketImpl()->secureWebSocketStream()->async_read(
            _webSocketMessage->buffer(),
            [this](const boost::system::error_code& error, size_t receivedBytes) {
                this->readComplete(error, receivedBytes);
            }
        );
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
