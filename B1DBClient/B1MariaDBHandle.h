//
// B1MariaDBHandle.h
//
// Library: B1DBClient
// Package: B1DBClient
// Module:  B1DBClient
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1DBCLIENT_MARIA_DB_HANDLE_H
#define _B1DBCLIENT_MARIA_DB_HANDLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/mysql/tcp.hpp>

namespace BnD {
    class B1IOContext;
    class B1SSLContext;
    class B1MariaDBHandle {
    protected:
        std::shared_ptr<B1IOContext> _context;                      // The I/O context to perform all operations.
        std::shared_ptr<boost::mysql::tcp_connection> _connection;  // Represents a single connection over TCP to a MySQL server.
        std::shared_ptr<B1SSLContext> _sslContext;
        std::shared_ptr<boost::mysql::tcp_ssl_connection> _sslConnection;
    public:
        bool initialize(bool useSSL = false);
        void finalize();
    public:
        B1IOContext* context() const { return _context.get(); }
        boost::mysql::tcp_connection* connection() const { return _connection.get(); }
        boost::mysql::tcp_ssl_connection* sslConnection() const { return _sslConnection.get(); }
        bool isOpened() const;
    };
}   //  !BnD

#endif  // !_B1DBCLIENT_MARIA_DB_HANDLE_H
