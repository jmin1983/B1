//
// B1HttpReadWriteImpl.cpp
//
// Library: B1Http
// Package: B1Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Http.h"
#include "B1HttpReadWriteImpl.h"
#include "B1HttpMessage.h"

#include <B1Network/B1BaseSocket.h>

#include <boost/bind/bind.hpp>

using namespace BnD;

B1HttpReadWriteImpl::B1HttpReadWriteImpl(B1BaseSocket* baseSocket, B1HttpReadWriteImplListener* listener)
    : B1BaseReadWriteImpl(baseSocket, listener)
    , _httpMessage(std::make_shared<B1HttpMessage>())
{
}

void B1HttpReadWriteImpl::writeResponseComplete(bool keepAlive, const boost::system::error_code& error, size_t transferredBytes)
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

bool B1HttpReadWriteImpl::implRead()
{
    _httpMessage->clearRequest();   // Make the request empty before reading, otherwise the operation behavior is undefined.
    try {
        boost::beast::http::async_read(*baseSocket()->asioSocket(), _httpMessage->buffer(), _httpMessage->request(),
                                       boost::bind(&B1HttpReadWriteImpl::readComplete, this,
                                                   boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
    catch (...) {
        return false;
    }
    return true;
}

bool B1HttpReadWriteImpl::implOnReadComplete(size_t receivedBytes)
{
    boost::ignore_unused(receivedBytes);
    if (auto l = listener()) {
        l->onReadComplete(*_httpMessage);
    }
    return false;   //  stop reading.
}

B1HttpReadWriteImplListener* B1HttpReadWriteImpl::listener() const
{
    return static_cast<B1HttpReadWriteImplListener*>(B1BaseReadWriteImpl::listener());
}

void B1HttpReadWriteImpl::writeResponse(boost::beast::http::message_generator&& response)
{
    bool keepAlive = response.keep_alive();
    boost::beast::async_write(*baseSocket()->asioSocket(), std::move(response),
                              boost::bind(&B1HttpReadWriteImpl::writeResponseComplete, this,
                                          keepAlive, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}
