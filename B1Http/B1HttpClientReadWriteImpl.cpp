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

#include <B1Network/B1BaseSocketImpl.h>

#include <boost/bind/bind.hpp>

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
    boost::beast::http::async_read(*httpSocketImpl()->stream(), _httpMessage->buffer(), _httpMessage->response(),
                                    boost::bind(&B1HttpClientReadWriteImpl::readComplete, this,
                                                boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
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
    httpSocketImpl()->stream()->expires_after(std::chrono::seconds(10));

    _httpMessage->clearRequest();
    //_httpMessage->request().version(11);
    _httpMessage->request().method(boost::beast::http::verb::get);
    _httpMessage->request().target(target.cString());
    _httpMessage->request().set(boost::beast::http::field::host, peerAddress().cString());
    _httpMessage->request().set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    boost::beast::http::async_write(*httpSocketImpl()->stream(), _httpMessage->request(),
                                    boost::bind(&B1HttpClientReadWriteImpl::writeRequestComplete, this,
                                                boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}
