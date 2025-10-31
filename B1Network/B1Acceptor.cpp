//
// B1Acceptor.cpp
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Network.h"
#include "B1Acceptor.h"
#include "B1AcceptorListener.h"
#include "B1IOContext.h"
#include "B1ServerSocket.h"

#include <B1Base/B1Thread.h>

#include <boost/bind/bind.hpp>

using namespace BnD;

B1Acceptor::B1Acceptor(const B1IOContext* context)
    : _context(context)
    , _acceptor()
    , _closed(true)
    , _listener(NULL)
{
}

B1Acceptor::~B1Acceptor()
{
}

void B1Acceptor::accept()
{
    if (_acceptor == NULL || _acceptor->is_open() != true) {
        _closed = true;
        return;
    }
    auto serverSocket = new B1ServerSocket(_context);
    _acceptor->async_accept(*serverSocket->asioSocket(), boost::bind(&B1Acceptor::acceptComplete, this, serverSocket, boost::asio::placeholders::error));
}

void B1Acceptor::acceptComplete(B1ServerSocket* serverSocket, const boost::system::error_code& error)
{
    auto sserverSocket = std::shared_ptr<B1ServerSocket>(serverSocket);
    if (error) {
        _listener->onAcceptFailed(sserverSocket, error.value());
    }
    else {
        _listener->onAcceptComplete(sserverSocket);
    }
    accept();
}

bool B1Acceptor::initialize(uint16 port, B1AcceptorListener* listener)
{
    if (_acceptor != NULL) {
        return false;
    }
    _acceptor = std::make_shared<boost::asio::ip::tcp::acceptor>(*_context->nativeContext(), boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
    _closed = false;
    _listener = listener;
    accept();
    return true;
}

void B1Acceptor::finalize()
{
    if (_acceptor) {
        B1LOG("closing acceptor");
        try {
            _acceptor->close();
        }
        catch (...) {}
        for (int32 i = 0; i < 300; ++i) {
            if (_closed) {
                break;
            }
            B1Thread::sleep(100);
        }
    }
    if (_closed) {
        B1LOG("acceptor closed");
    }
    else {
        B1LOG("acceptor not closed -> force close");
        _closed = true;
    }
    _acceptor.reset();
    _listener = NULL;
}
