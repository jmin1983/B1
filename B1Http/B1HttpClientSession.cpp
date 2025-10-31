//
// B1HttpClientSession.cpp
//
// Library: B1Http
// Package: Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Http.h"
#include "B1HttpClientSession.h"
#include "B1HttpMessage.h"

using namespace BnD;

B1HttpClientSession::B1HttpClientSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* baseClientSessionListener)
    : B1BaseClientSession(clientSocket, baseClientSessionListener)
{
}

B1HttpClientSession::~B1HttpClientSession()
{
}

void B1HttpClientSession::onReadComplete(const B1HttpClientMessage& httpMessage)
{
    onHttpResponseSuccess(httpMessage.response().body().c_str());
}

void B1HttpClientSession::onReadFailed(int32 reason)
{
    onHttpResponseFail(reason);
    setSessionStatusDisconnecting(reason);
}

void B1HttpClientSession::onWriteFailed(int32 reason)
{
    onHttpResponseFail(reason);
    setSessionStatusDisconnecting(reason);
}

auto B1HttpClientSession::createReadWriteImpl() -> B1BaseReadWriteImpl*
{
    return new B1HttpClientReadWriteImpl(this);
}

auto B1HttpClientSession::readWriteImpl() const -> B1HttpClientReadWriteImpl*
{
    return static_cast<B1HttpClientReadWriteImpl*>(B1BaseClientSession::readWriteImpl());
}

void B1HttpClientSession::sendHttpRequest(const B1String& target)
{
    readWriteImpl()->writeRequest(target);
}
