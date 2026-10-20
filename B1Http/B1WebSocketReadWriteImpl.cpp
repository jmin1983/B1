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
    , _maxDataSize(CONSTS_DEFAULT_MAX_DATA_SIZE)
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
        if (_owner->isOpen()) {
            _readyToWrite = false;
            if (textDataBunch.isEmpty() != true) {
                _owner->writeText(std::move(textDataBunch));
            }
            else if (binaryData.empty() != true) {
                _owner->writeBinary(std::move(binaryData));
            }
        }
    }
}

void B1WebSocketReadWriteImpl::Writer::implThreadEnd()
{
    const int32 timeoutCount = 3000;
    for (int32 i = 0; i < timeoutCount; ++i) {
        if (_owner->isOpen()) {
            B1Thread::sleep(1);
        }
        else {
            break;
        }
    }
    if (_owner->isOpen()) {
        B1LOG("session still opened!!! -> can make a crash!!!");
        B1Thread::sleep(2000);
    }
}

void B1WebSocketReadWriteImpl::Writer::addWriteBinary(std::vector<uint8>&& data)
{
    B1AutoLock al(_lock);
    if (_data.size() > _maxDataSize) {
        _data.pop_front();
    }
    _data.push_back(std::make_shared<BinaryData>(std::move(data)));
}

void B1WebSocketReadWriteImpl::Writer::addWriteText(B1String&& text)
{
    B1AutoLock al(_lock);
    if (_data.size() > _maxDataSize) {
        _data.pop_front();
    }
    _data.push_back(std::make_shared<TextData>(std::move(text)));
}

void B1WebSocketReadWriteImpl::implWriteBinary(std::vector<uint8>&& data)
{
    webSocketImpl()->webSocketStream()->binary(true);

    auto temp = std::make_shared<std::vector<uint8> >(std::move(data));
    webSocketImpl()->webSocketStream()->async_write(\
        boost::beast::net::buffer(*temp),
        [this, temp](const boost::system::error_code& error, size_t transferredBytes) {
            this->writeBinaryComplete(temp, error, transferredBytes);
        }
    );
}

void B1WebSocketReadWriteImpl::implWriteText(B1String&& data)
{
    webSocketImpl()->webSocketStream()->text(true);

    auto temp = std::make_shared<std::string>(std::move(const_cast<std::string&>(data.to_string())));
    webSocketImpl()->webSocketStream()->async_write(
        boost::beast::net::buffer(*temp),
        [this, temp](const boost::system::error_code& error, size_t transferredBytes) {
            this->writeTextComplete(temp, error, transferredBytes);
        }
    );
}

B1BaseSocketImpl* B1WebSocketReadWriteImpl::createBaseSocketImpl()
{
    return new B1WebSocketImpl();
}

bool B1WebSocketReadWriteImpl::implRead()
{
    try {
        webSocketImpl()->webSocketStream()->async_read(
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

void B1WebSocketReadWriteImpl::writeBinaryComplete(std::shared_ptr<std::vector<uint8> > data, const boost::system::error_code& error, size_t transferredBytes)
{
    _writer.writeComplete();
    if (error) {
        B1BaseReadWriteImpl::writeComplete(error, transferredBytes);
    }
    else {
        if (auto l = listener()) {
            l->onWebSocketWriteComplete(transferredBytes);
        }
    }
}

void B1WebSocketReadWriteImpl::writeTextComplete(std::shared_ptr<std::string> text, const boost::system::error_code& error, size_t transferredBytes)
{
    _writer.writeComplete();
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
