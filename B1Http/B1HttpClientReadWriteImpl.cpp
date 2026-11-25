//
// B1HttpClientReadWriteImpl.cpp
//
// Library: B1Http
// Package: Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Http.h"
#include "B1HttpClientReadWriteImpl.h"
#include "B1HttpMessage.h"
#include "B1HttpSocketImpl.h"

#include <B1Base/B1Lock.h>
#include <B1Network/B1BaseSocketImpl.h>

using namespace BnD;

B1HttpClientReadWriteImpl::B1HttpClientReadWriteImpl(B1HttpClientReadWriteImplListener* listener)
    : B1BaseReadWriteImpl(listener)
    , _httpMessage(std::make_shared<B1HttpClientMessage>())
{
}

auto B1HttpClientReadWriteImpl::httpSocketImpl() const -> B1HttpSocketImpl*
{
    return static_cast<B1HttpSocketImpl*>(baseSocketImpl());
}

void B1HttpClientReadWriteImpl::writeRequestComplete(const boost::system::error_code& error, size_t transferredBytes)
{
    if (error) {
        B1BaseReadWriteImpl::writeComplete(error, transferredBytes);
    }
}

B1BaseSocketImpl* B1HttpClientReadWriteImpl::createBaseSocketImpl()
{
    return new B1HttpSocketImpl();
}

bool B1HttpClientReadWriteImpl::implRead()
{
    _httpMessage->clearResponse();
    boost::beast::http::async_read(
        *httpSocketImpl()->stream(), _httpMessage->buffer(), _httpMessage->response(),
        [this](const boost::system::error_code& error, size_t receivedBytes) {
            this->readComplete(error, receivedBytes);
        }
    );
    return true;
}

bool B1HttpClientReadWriteImpl::implOnReadComplete(size_t receivedBytes)
{
    boost::ignore_unused(receivedBytes);
    if (auto l = listener()) {
        l->onReadComplete(*_httpMessage);
    }
    return true;
}

auto B1HttpClientReadWriteImpl::listener() const -> B1HttpClientReadWriteImplListener*
{
    return static_cast<B1HttpClientReadWriteImplListener*>(B1BaseReadWriteImpl::listener());
}

void B1HttpClientReadWriteImpl::writeRequest(const B1String& target)
{
    auto requestTarget = std::make_shared<std::string>(target.to_string());
    auto socketImpl = httpSocketImpl();
    if (NULL == socketImpl) {
        return;
    }
    B1AutoLock al(*socketImpl->closeLock());   //  stream can not be cleaned up while using.
    auto stream = socketImpl->stream();
    if (NULL == stream) {   //  cleaned up.
        return;
    }
    //  beast stream is not thread-safe: start async_write in the io thread(same thread as async_read).
    boost::asio::post(stream->get_executor(), [this, requestTarget]() {
        auto socketImplInIO = httpSocketImpl();
        if (NULL == socketImplInIO) {
            return;
        }
        const B1String host = peerAddress();    //  before locking(peerAddress() also locks).
        B1AutoLock alInIO(*socketImplInIO->closeLock());
        auto streamInIO = socketImplInIO->stream();
        if (NULL == streamInIO) {
            return;
        }
        streamInIO->expires_after(std::chrono::seconds(10));
        _httpMessage->clearRequest();
        //_httpMessage->request().version(11);
        _httpMessage->request().method(boost::beast::http::verb::get);
        _httpMessage->request().target(*requestTarget);
        _httpMessage->request().set(boost::beast::http::field::host, host.cString());
        _httpMessage->request().set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        boost::beast::http::async_write(
            *streamInIO, _httpMessage->request(),
            [this](const boost::system::error_code& error, size_t transferredBytes) {
                this->writeRequestComplete(error, transferredBytes);
            }
        );
    });
}
