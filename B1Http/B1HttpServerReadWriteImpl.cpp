//
// B1HttpServerReadWriteImpl.cpp
//
// Library: B1Http
// Package: Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Http.h"
#include "B1HttpServerReadWriteImpl.h"
#include "B1HttpMessage.h"

#include <B1Network/B1BaseSocketImpl.h>

#include <boost/bind/bind.hpp>

using namespace BnD;

B1HttpServerReadWriteImpl::B1HttpServerReadWriteImpl(B1HttpServerReadWriteImplListener* listener)
    : B1BaseReadWriteImpl(listener)
    , _httpMessage(std::make_shared<B1HttpServerMessage>())
{
}

auto B1HttpServerReadWriteImpl::asioSocketImpl() const -> B1ASIOSocketImpl*
{
    return static_cast<B1ASIOSocketImpl*>(baseSocketImpl());
}

void B1HttpServerReadWriteImpl::writeResponseComplete(bool keepAlive, const boost::system::error_code& error, size_t transferredBytes)
{
    if (error) {
        B1BaseReadWriteImpl::writeComplete(error, transferredBytes);
    }
    else {
        if (auto l = listener()) {
            l->onWriteComplete(keepAlive, transferredBytes);
        }
    }
}

void B1HttpServerReadWriteImpl::implWriteResponse(boost::beast::http::message_generator&& response)
{
    bool keepAlive = response.keep_alive();
    boost::beast::async_write(*asioSocketImpl()->getASIOSocket(), std::move(response),
                              boost::bind(&B1HttpServerReadWriteImpl::writeResponseComplete, this,
                                          keepAlive, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

bool B1HttpServerReadWriteImpl::implRead()
{
    _httpMessage->clearRequest();   // Make the request empty before reading, otherwise the operation behavior is undefined.
    try {
        boost::beast::http::async_read(*asioSocketImpl()->getASIOSocket(), _httpMessage->buffer(), _httpMessage->request(),
                                       boost::bind(&B1HttpServerReadWriteImpl::readComplete, this,
                                                   boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
    catch (...) {
        return false;
    }
    return true;
}

bool B1HttpServerReadWriteImpl::implOnReadComplete(size_t receivedBytes)
{
    boost::ignore_unused(receivedBytes);
    if (auto l = listener()) {
        l->onReadComplete(*_httpMessage);
    }
    return false;   //  stop reading.
}

auto B1HttpServerReadWriteImpl::listener() const -> B1HttpServerReadWriteImplListener*
{
    return static_cast<B1HttpServerReadWriteImplListener*>(B1BaseReadWriteImpl::listener());
}

void B1HttpServerReadWriteImpl::writeResponse(boost::beast::http::message_generator&& response)
{
    implWriteResponse(std::move(response));
}
