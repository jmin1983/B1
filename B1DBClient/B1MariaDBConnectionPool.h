//
// B1MariaDBConnectionPool.h
//
// Library: B1DBClient
// Package: B1DBClient
// Module:  B1DBClient
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1DBCLIENT_MARIA_DB_CONNECTION_POOL_H
#define _B1DBCLIENT_MARIA_DB_CONNECTION_POOL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <map>

namespace BnD {
    class B1MariaDBHandle;
    class B1MariaDBConnectionPool {
    public:
        B1MariaDBConnectionPool();
        virtual ~B1MariaDBConnectionPool();
    protected:
        enum CONSTS {
            CONSTS_MAX_CONNECTIONS = 150,
        };
    private:
        B1String _address;
        B1String _dbName;
        B1String _user;
        B1String _password;
        uint16 _port;
        bool _useSSL;
    protected:
        std::shared_ptr<class B1Lock> _lock;
        std::map<std::shared_ptr<B1MariaDBHandle>, bool> _handles;  //  map<handle, now_using>
    protected:
        auto createNewHandle() const -> std::shared_ptr<B1MariaDBHandle>;
    public:
        bool initialize(B1String&& address, uint16 port, B1String&& dbName, B1String&& user, B1String&& password, uint32 connectionCount, bool useSSL = false);
        void finalize();
        void checkConnections();
        int32 currentConnections() const;
        //bool increaseConnections(int32 count);
        //void decreaseConnections(int32 count);
        auto getHandle(int32 trys = 30) -> std::shared_ptr<B1MariaDBHandle>;    //  try to get handle every second. wait if no handle available. return NULL if failed.
        void releaseHandle(std::shared_ptr<B1MariaDBHandle> handle);
    };
}   //  !BnD

#endif  // !_B1DBCLIENT_MARIA_DB_CONNECTION_POOL_H
