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

using namespace BnD;

bool B1MariaDBHandle::initialize(bool useSSL)
{
    if (_context || _connection || _sslConnection) {
        return false;
    }
    _context.reset(new boost::asio::io_context());
    if (useSSL) {
        _sslContext.reset(new boost::asio::ssl::context(boost::asio::ssl::context::tls_client));
        _sslConnection.reset(new boost::mysql::tcp_ssl_connection(_context->get_executor(), *_sslContext));
    }
    else {
        _connection.reset(new boost::mysql::tcp_connection(*_context));
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