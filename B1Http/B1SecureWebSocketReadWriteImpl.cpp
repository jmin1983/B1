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

#include <B1Base/B1Lock.h>

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
    auto temp = std::make_shared<std::vector<uint8> >(std::move(data));
    if (auto socketImpl = secureWebSocketImpl()) {
        B1AutoLock al(*socketImpl->closeLock());   //  stream can not be cleaned up while using.
        if (auto stream = socketImpl->secureWebSocketStream()) {
            //  beast stream is not thread-safe: start async_write in the io thread(same thread as async_read).
            boost::asio::post(stream->get_executor(), [this, temp]() {
                if (auto socketImplInIO = secureWebSocketImpl()) {
                    B1AutoLock alInIO(*socketImplInIO->closeLock());
                    if (auto streamInIO = socketImplInIO->secureWebSocketStream()) {
                        streamInIO->binary(true);
                        streamInIO->async_write(
                            boost::beast::net::buffer(*temp),
                            [this, temp](const boost::system::error_code& error, size_t transferredBytes) {
                                this->writeBinaryComplete(temp, error, transferredBytes);
                            }
                        );
                        return;
                    }
                }
                this->writeBinaryComplete(temp, boost::asio::error::operation_aborted, 0);  //  cleaned up while posting(call after unlock).
            });
            return;
        }
    }
    //  cleaned up(called in the writer thread). do not notify the listener: the session may be being destroyed.
    B1LOG("socket already cleaned up -> ignore writing: size[%u]", static_cast<uint32>(temp->size()));
    _writer.writeComplete();
}

void B1SecureWebSocketReadWriteImpl::implWriteText(B1String&& data)
{
    auto temp = std::make_shared<std::string>(std::move(const_cast<std::string&>(data.to_string())));
    if (auto socketImpl = secureWebSocketImpl()) {
        B1AutoLock al(*socketImpl->closeLock());   //  stream can not be cleaned up while using.
        if (auto stream = socketImpl->secureWebSocketStream()) {
            //  beast stream is not thread-safe: start async_write in the io thread(same thread as async_read).
            boost::asio::post(stream->get_executor(), [this, temp]() {
                if (auto socketImplInIO = secureWebSocketImpl()) {
                    B1AutoLock alInIO(*socketImplInIO->closeLock());
                    if (auto streamInIO = socketImplInIO->secureWebSocketStream()) {
                        streamInIO->text(true);
                        streamInIO->async_write(
                            boost::beast::net::buffer(*temp),
                            [this, temp](const boost::system::error_code& error, size_t transferredBytes) {
                                this->writeTextComplete(temp, error, transferredBytes);
                            }
                        );
                        return;
                    }
                }
                this->writeTextComplete(temp, boost::asio::error::operation_aborted, 0);  //  cleaned up while posting(call after unlock).
            });
            return;
        }
    }
    //  cleaned up(called in the writer thread). do not notify the listener: the session may be being destroyed.
    B1LOG("socket already cleaned up -> ignore writing: size[%u]", static_cast<uint32>(temp->size()));
    _writer.writeComplete();
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
