//
// B1WebSocketReadWriteImpl.cpp
//
// Library: B1Http
// Package: Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Http.h"
#include "B1WebSocketReadWriteImpl.h"
#include "B1WebSocketImpl.h"
#include "B1WebSocketMessage.h"

#include <B1Network/B1BaseSocket.h>

#include <boost/bind/bind.hpp>

using namespace BnD;

B1WebSocketReadWriteImpl::B1WebSocketReadWriteImpl(B1WebSocketReadWriteImplListener* listener)
    : B1BaseReadWriteImpl(listener)
    , _writer()
    , _webSocketMessage(std::make_shared<B1WebSocketMessage>())
{
    _writer.setOwner(this);
    _writer.run();
}

B1WebSocketReadWriteImpl::~B1WebSocketReadWriteImpl()
{
    _writer.quit();
    _writer.join();
}

void B1WebSocketReadWriteImpl::Writer::implLooperFunc()
{
    B1String textData;
    std::vector<uint8> binaryData;
    {
        B1AutoLock al(_lock);
        if (_textData.empty() != true) {
            textData = std::move(_textData.front());
            _textData.pop_front();
        }
        if (textData.isEmpty()) {
            if (_binaryData.empty() != true) {
                binaryData = std::move(_binaryData.front());
                _binaryData.pop_front();
            }
        }
    }
    if (textData.isEmpty() && binaryData.empty()) {
        B1Thread::sleep(10);
    }
    else {
        while (_readyToWrite != true) {
            if (isQuitRequested()) {
                return;
            }
            B1Thread::sleep(1);
        }
        _readyToWrite = false;
        if (textData.isEmpty() != true) {
            _owner->writeText(std::move(textData));
        }
        else if (binaryData.empty() != true) {
            _owner->writeBinary(std::move(binaryData));
        }
    }
}

void B1WebSocketReadWriteImpl::Writer::addWriteBinary(std::vector<uint8>&& data)
{
    B1AutoLock al(_lock);
    _binaryData.push_back(std::move(data));
}

void B1WebSocketReadWriteImpl::Writer::addWriteText(B1String&& text)
{
    B1AutoLock al(_lock);
    _textData.push_back(std::move(text));
}

bool B1WebSocketReadWriteImpl::Writer::isBinaryDataEmpty() const
{
    B1AutoLock al(_lock);
    return _binaryData.empty();
}

bool B1WebSocketReadWriteImpl::Writer::isTextDataEmpty() const
{
    B1AutoLock al(_lock);
    return _textData.empty();
}

void B1WebSocketReadWriteImpl::implWriteBinary(std::vector<uint8>&& data)
{
    webSocketImpl()->webSocketStream()->binary(true);

    auto temp = new std::vector<uint8>(std::move(data));
    webSocketImpl()->webSocketStream()->async_write(boost::beast::net::buffer(*temp),
                                                    boost::bind(&B1WebSocketReadWriteImpl::writeBinaryComplete, this,
                                                                temp, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void B1WebSocketReadWriteImpl::implWriteText(B1String&& data)
{
    webSocketImpl()->webSocketStream()->text(true);

    auto temp = new std::string(std::move(const_cast<std::string&>(data.to_string())));
    webSocketImpl()->webSocketStream()->async_write(boost::beast::net::buffer(*temp),
                                                    boost::bind(&B1WebSocketReadWriteImpl::writeTextComplete, this,
                                                                temp, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

B1BaseSocketImpl* B1WebSocketReadWriteImpl::createBaseSocketImpl()
{
    return new B1WebSocketImpl();
}

bool B1WebSocketReadWriteImpl::implRead()
{
    try {
        webSocketImpl()->webSocketStream()->async_read(_webSocketMessage->buffer(),
                                                       boost::bind(&B1WebSocketReadWriteImpl::readComplete, this,
                                                                   boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
    catch (...) {
        return false;
    }
    return true;
}

bool B1WebSocketReadWriteImpl::implOnReadComplete(size_t receivedBytes)
{
    bool continueReading = false;
    boost::ignore_unused(receivedBytes);
    if (auto l = listener()) {
        if (webSocketImpl()->webSocketStream()->got_text()) {
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

auto B1WebSocketReadWriteImpl::listener() const -> B1WebSocketReadWriteImplListener*
{
    return static_cast<B1WebSocketReadWriteImplListener*>(B1BaseReadWriteImpl::listener());
}

void B1WebSocketReadWriteImpl::writeBinary(std::vector<uint8>&& data)
{
    implWriteBinary(std::move(data));
}

void B1WebSocketReadWriteImpl::writeText(B1String&& text)
{
    implWriteText(std::move(text));
}

void B1WebSocketReadWriteImpl::writeBinaryComplete(std::vector<uint8>* data, const boost::system::error_code& error, size_t transferredBytes)
{
    _writer.writeComplete();
    delete data;
    if (error) {
        B1BaseReadWriteImpl::writeComplete(error, transferredBytes);
    }
    else {
        if (auto l = listener()) {
            l->onWebSocketWriteComplete(transferredBytes);
        }
    }
}

void B1WebSocketReadWriteImpl::writeTextComplete(std::string* text, const boost::system::error_code& error, size_t transferredBytes)
{
    _writer.writeComplete();
    delete text;
    if (error) {
        B1BaseReadWriteImpl::writeComplete(error, transferredBytes);
    }
    else {
        if (auto l = listener()) {
            l->onWebSocketWriteComplete(transferredBytes);
        }
    }
}

void B1WebSocketReadWriteImpl::addWriteBinary(std::vector<uint8>&& data)
{
    _writer.addWriteBinary(std::move(data));
}

void B1WebSocketReadWriteImpl::addWriteText(B1String&& text)
{
    _writer.addWriteText(std::move(text));
}

bool B1WebSocketReadWriteImpl::isBinaryDataEmpty() const
{
    return _writer.isBinaryDataEmpty();
}

bool B1WebSocketReadWriteImpl::isTextDataEmpty() const
{
    return _writer.isTextDataEmpty();
}

auto B1WebSocketReadWriteImpl::webSocketImpl() const ->B1WebSocketImpl*
{
    return static_cast<B1WebSocketImpl*>(baseSocketImpl());
}
