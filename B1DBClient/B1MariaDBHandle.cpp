//
// B1MariaDBHandle.cpp
//
// Library: B1DBClient
// Package: B1DBClient
// Module:  B1DBClient
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1DBClient.h"
#include "B1MariaDBHandle.h"

#include <B1Network/B1IOContext.h>
#include <B1Network/B1SSLContext.h>

#include <boost/asio/ip/tcp.hpp>

using namespace BnD;

bool B1MariaDBHandle::initialize(bool useSSL)
{
    if (_context || _connection || _sslConnection) {
        return false;
    }
    _context.reset(new B1IOContext(1));
    if (useSSL) {
        _sslContext = std::make_shared<B1SSLContext>(false);
        _sslConnection = std::make_shared<boost::mysql::tcp_ssl_connection>(_context->nativeContext()->get_executor(), *_sslContext->nativeContext());
    }
    else {
        _connection = std::make_shared<boost::mysql::tcp_connection>(*_context->nativeContext());
    }
    return true;
}

void B1MariaDBHandle::finalize()
{
    if (_sslConnection) {
        assert(_connection == NULL && _sslContext != NULL);
        try {
            _sslConnection->close();
        }
        catch (...) {}
        _sslConnection.reset();
    }
    if (_connection) {
        assert(_sslConnection == NULL && _sslContext == NULL);
        try {
            _connection->close();
        }
        catch (...) {}
        _connection.reset();
    }
    if (_sslContext) {
        _sslContext.reset();
    }
    if (_context) {
        _context.reset();
    }
}

bool B1MariaDBHandle::isOpened() const
{
    if (_connection) {
        return _connection->stream().is_open();
    }
    else if (_sslConnection) {
        return _sslConnection->stream().lowest_layer().is_open();
    }
    return false;
}