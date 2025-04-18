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
#include <B1Util/B1UtilMacro.h>

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

B1WebSocketReadWriteImpl::Writer::Writer()
    : _owner(NULL)
    , _readyToWrite(true)
    , _textBunchHint(CONSTS_DEFAULT_TEXT_BUNCH_HINT)
{
}

void B1WebSocketReadWriteImpl::Writer::implLooperFunc()
{
    B1String textDataBunch;
    std::vector<uint8> binaryData;
    {
        B1AutoLock al(_lock);
        if (_data.empty() != true) {
            if (_data.front()->isBinaryData()) {
                binaryData.swap(static_cast<BinaryData*>(_data.front().get())->_binary);
                _data.pop_front();
            }
            else {
                textDataBunch = "[";
                bool first = true;
                SAFE_CONTAINER_FOREACH(_data, itr) {
                    if (itr->get()->isBinaryData()) {
                        break;
                    }
                    if (first) {
                        first = false;
                    }
                    else {
                        textDataBunch.append(",");
                    }
                    textDataBunch.append(static_cast<TextData*>(itr->get())->_text);
                    _data.erase(itr);
                    if (textDataBunch.length() > _textBunchHint) {
                        break;
                    }
                }
                textDataBunch.append("]");
            }
        }
    }
    if (textDataBunch.isEmpty() && binaryData.empty()) {
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
        if (textDataBunch.isEmpty() != true) {
            _owner->writeText(std::move(textDataBunch));
        }
        else if (binaryData.empty() != true) {
            _owner->writeBinary(std::move(binaryData));
        }
    }
}

void B1WebSocketReadWriteImpl::Writer::addWriteBinary(std::vector<uint8>&& data)
{
    B1AutoLock al(_lock);
    _data.push_back(std::make_shared<BinaryData>(std::move(data)));
}

void B1WebSocketReadWriteImpl::Writer::addWriteText(B1String&& text)
{
    B1AutoLock al(_lock);
    _data.push_back(std::make_shared<TextData>(std::move(text)));
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

void B1WebSocketReadWriteImpl::setTextBunchHint(uint32 value)
{
    _writer.setTextBunchHint(value);
}

auto B1WebSocketReadWriteImpl::webSocketImpl() const ->B1WebSocketImpl*
{
    return static_cast<B1WebSocketImpl*>(baseSocketImpl());
}
