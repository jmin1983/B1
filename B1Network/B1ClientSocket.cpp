//
// B1ClientSocket.cpp
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Network.h"
#include "B1ClientSocket.h"
#include "B1ClientSocketListener.h"
#include "B1IOContext.h"
#include "B1IOLooper.h"

#include <boost/bind/bind.hpp>

using namespace BnD;

B1ClientSocket::B1ClientSocket()
    : B1BaseSocket()
    , _ioLooper(new B1IOLooper())
    , _listener(NULL)
    , _address()
    , _port(0)
{
    _ioLooper->startup();
    resetSocket(_ioLooper->ioContext());
}

B1ClientSocket::~B1ClientSocket()
{
    _ioLooper->shutdown();
}

void B1ClientSocket::connectComplete(const boost::system::error_code& error)
{
    if (error) {
        try {
            if (isOpen()) {
                close();
            }
        }
        catch (...) {
            B1LOG("catch on close");
        }
        if (_listener) {
            _listener->onConnectFailed(error.value());
        }
    }
    else {
        if (_listener) {
            _listener->onConnectComplete();
        }
    }
}

bool B1ClientSocket::connectAsync()
{
    B1LOG("connecting: address[%s], port[%d]", _address.cString(), _port);
    if (_address.isEmpty()) {
        return false;
    }
    try {
        if (_ioLooper->pause() != true) {
            _ioLooper.reset(new B1IOLooper());
            _ioLooper->startup();
            resetSocket(_ioLooper->ioContext());
        }
        boost::asio::ip::tcp::resolver resolver(*_ioLooper->ioContext()->nativeContext());
        char port[10] = {0};
        sprintf(port, "%d", _port);
        auto result = resolver.resolve(_address.cString(), port);
        if (result.empty()) {
            return false;
        }
        asioSocket()->async_connect(result.begin()->endpoint(), boost::bind(&B1ClientSocket::connectComplete, this, boost::asio::placeholders::error));
        _ioLooper->resume();
    }
    catch (...) {
        return false;
    }
    return true;
}

bool B1ClientSocket::connect(B1String&& address, uint16 port, B1ClientSocketListener* listener)
{
    _address = std::move(address);
    _port = port;
    _listener = listener;
    return connectAsync();
}

bool B1ClientSocket::reconnect()
{
    return connectAsync();
}
