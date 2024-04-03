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

namespace BnD {
    class B1MariaDBHandle;
    class B1MariaDBResult;
    class B1MariaDBClient {
    public:
        B1MariaDBClient();
        virtual ~B1MariaDBClient();
    public:
        bool execute(B1MariaDBHandle* handle, const B1String& sql, B1MariaDBResult* result = NULL) const;
        bool executeBatch(B1MariaDBHandle* handle, const B1String& sql, B1MariaDBResult* result = NULL) const;
        bool executePrepared(B1MariaDBHandle* handle, const B1String& stmt,
                             const std::vector<B1String>& bind0, const std::vector<B1String>& bind1,
                             const std::vector<std::vector<uint8> >& bind2, const std::vector<std::vector<uint8> >& bind3, const std::vector<std::vector<uint8> >& bind4) const;
    };
}   //  !BnD

#endif  // !_B1DBCLIENT_MARIA_DB_CLIENT_H
