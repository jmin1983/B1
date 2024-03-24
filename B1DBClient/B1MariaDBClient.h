//
// B1MariaDBClient.h
//
// Library: B1DBClient
// Package: B1DBClient
// Module:  B1DBClient
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1DBCLIENT_MARIA_DB_CLIENT_H
#define _B1DBCLIENT_MARIA_DB_CLIENT_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <map>

namespace BnD {
    class B1MariaDBHandle;
    class B1MariaDBResult;
    class B1MariaDBClient {
    public:
        B1MariaDBClient();
        virtual ~B1MariaDBClient();
    protected:
        std::shared_ptr<B1MariaDBHandle> _handle;
    public:
        bool initialize(const B1String& address, uint16 port, const B1String& dbName, const B1String& user, const B1String& password, bool useSSL = false);
        void finalize();
        bool execute(const B1String& sql, B1MariaDBResult* result = NULL) const;
        bool executeBatch(const B1String& sql, B1MariaDBResult* result = NULL) const;
        bool executePrepared(const B1String& stmt,
                             const std::vector<B1String>& bind0, const std::vector<B1String>& bind1,
                             const std::vector<std::vector<uint8> >& bind2, const std::vector<std::vector<uint8> >& bind3, const std::vector<std::vector<uint8> >& bind4) const;
    };
}   //  !BnD

#endif  // !_B1DBCLIENT_MARIA_DB_CLIENT_H
