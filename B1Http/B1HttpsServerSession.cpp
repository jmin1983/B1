//
// B1HttpsServerSession.cpp
//
// Library: B1Http
// Package: Https
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Http.h"
#include "B1HttpsServerSession.h"
#include "B1HttpsReadWriteImpl.h"
#include "B1HttpMessage.h"
#include "B1HttpResponseGenerator.h"
#include "B1SecureSocketImpl.h"

using namespace BnD;

B1HttpsServerSession::B1HttpsServerSession(B1ServerSocket* serverSocket, B1BaseServerSessionListener* baseServerSessionListener,
                                           B1HttpServerSessionListener* httpServerSessionListener, const B1String& contentsRootPath, const B1SSLContext& sslContext)
    : B1HttpServerSession(serverSocket, baseServerSessionListener, httpServerSessionListener, contentsRootPath)
    , _sslContext(sslContext)
{
}

B1HttpsServerSession::~B1HttpsServerSession()
{
}

auto B1HttpsServerSession::createReadWriteImpl() -> B1BaseReadWriteImpl*
{
    return new B1HttpsReadWriteImpl(_sslContext, this);
}

auto B1HttpsServerSession::readWriteImpl() const -> B1HttpsReadWriteImpl*
{
    return static_cast<B1HttpsReadWriteImpl*>(B1BaseServerSession::readWriteImpl());
}
