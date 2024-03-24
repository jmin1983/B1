//
// B1HttpServerSession.cpp
//
// Library: B1Http
// Package: B1Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Http.h"
#include "B1HttpServerSession.h"
#include "B1HttpMessage.h"
#include "B1HttpResponseGenerator.h"
#include "B1HttpServerSessionListener.h"

using namespace BnD;

B1HttpServerSession::B1HttpServerSession(B1ServerSocket* serverSocket, B1BaseServerSessionListener* baseServerSessionListener,
                                         B1HttpServerSessionListener* httpServerSessionListener, const B1String& contentsRootPath)
    : B1BaseServerSession(serverSocket, baseServerSessionListener)
    , _httpServerSessionListener(httpServerSessionListener)
    , _contentsRootPath(contentsRootPath)
    , _responseGenerator(std::make_shared<B1HttpResponseGenerator>())
{
}

B1HttpServerSession::~B1HttpServerSession()
{
}

void B1HttpServerSession::onReadComplete(const B1HttpMessage& httpMessage)
{
    if (boost::beast::websocket::is_upgrade(httpMessage.request())) {
        _httpServerSessionListener->onHttpServerSessionUpgradeRequested(serverSocket(), httpMessage);
    }
    else {
        auto response = _responseGenerator->makeResponse(httpMessage, _contentsRootPath);
        readWriteImpl()->writeResponse(std::move(response));
    }
}

void B1HttpServerSession::onWriteComplete(bool keepAlive, size_t dataSize)
{
    if (keepAlive != true) {
        disconnect();
    }
    else {
        B1BaseSession::beginRead();
    }
}

void B1HttpServerSession::onReadFailed(int32 reason)
{
    setSessionStatusDisconnected(reason);
}

void B1HttpServerSession::onWriteFailed(int32 reason)
{
    setSessionStatusDisconnected(reason);
}

B1BaseReadWriteImpl* B1HttpServerSession::createReadWriteImpl(B1BaseSocket* baseSocket)
{
    return new B1HttpReadWriteImpl(baseSocket, this);
}

B1HttpReadWriteImpl* B1HttpServerSession::readWriteImpl() const
{
    return static_cast<B1HttpReadWriteImpl*>(B1BaseServerSession::readWriteImpl());
}
