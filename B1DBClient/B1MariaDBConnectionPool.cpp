//
// B1MariaDBConnectionPool.cpp
//
// Library: B1DBClient
// Package: B1DBClient
// Module:  B1DBClient
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1DBClient.h"
#include "B1MariaDBConnectionPool.h"
#include "B1MariaDBHandle.h"

#include <B1Base/B1Lock.h>
#include <B1Base/B1Thread.h>
#include <B1Network/B1IOContext.h>
#include <B1Util/B1UtilMacro.h>

using namespace BnD;

B1MariaDBConnectionPool::B1MariaDBConnectionPool()
    : _address()
    , _dbName()
    , _user()
    , _password()
    , _port(0)
    , _useSSL(false)
    , _connectionCount(0)
    , _lock(std::make_shared<B1Lock>())
    , _initialized(false)
    , _handles()
{
}

B1MariaDBConnectionPool::~B1MariaDBConnectionPool()
{
}

auto B1MariaDBConnectionPool::createNewHandle() const -> std::shared_ptr<B1MariaDBHandle>
{
    auto handle = std::make_shared<B1MariaDBHandle>();
    if (handle->initialize(_useSSL) != true) {
        handle->finalize();
        handle.reset();
        return std::shared_ptr<B1MariaDBHandle>();
    }
    try {
        boost::asio::ip::tcp::resolver resolver(handle->context()->nativeContext()->get_executor());
        char portString[10] = {0};
        sprintf(portString, "%d", _port);
        auto endpoints = resolver.resolve(_address.cString(), portString);
        boost::mysql::handshake_params params(_user.cString(), _password.cString(), _dbName.cString());
        if (_useSSL) {
            handle->sslConnection()->connect(*endpoints.begin(), params);
        }
        else {
            handle->connection()->connect(*endpoints.begin(), params);
        }
    }
    catch (const boost::mysql::error_with_diagnostics& err) {
        B1LOG("boost_my_sql_error: code[%d], msg[%s]", err.code().value(), err.what());
        handle->finalize();
        handle.reset();
        return std::shared_ptr<B1MariaDBHandle>();
    }
    catch (...) {
        B1LOG("unknown error in boost_my_sql");
        handle->finalize();
        handle.reset();
        return std::shared_ptr<B1MariaDBHandle>();
    }
    return handle;
}

bool B1MariaDBConnectionPool::initialize(B1String&& address, uint16 port, B1String&& dbName, B1String&& user, B1String&& password, uint32 connectionCount, bool useSSL)
{
    if (CONSTS_MAX_CONNECTIONS <= connectionCount) {
        B1LOG("too many connection requested: count[%u], max[%u]", connectionCount, CONSTS_MAX_CONNECTIONS);
        return false;
    }
    {
        B1AutoLock al(*_lock);
        if (_initialized) {
            return false;
        }
        _address = std::move(address);
        _port = port;
        _dbName = std::move(dbName);
        _user = std::move(user);
        _password = std::move(password);
        _useSSL = useSSL;
        _connectionCount = connectionCount;
        _initialized = true;    //  set before creating connections and kept even if failed(caller calls finalize()).
    }
    for (uint32 i = 0; i < connectionCount; ++i) {
        auto newHandle = createNewHandle();
        if (newHandle == nullptr) {
            B1LOG("unable to create new handle: index[%u], total[%u]", i, connectionCount);
            return false;
        }
        B1AutoLock al(*_lock);
        _handles.insert(std::make_pair(newHandle, false));
    }
    return true;
}

void B1MariaDBConnectionPool::finalize()
{
    {
        B1AutoLock al(*_lock);
        _initialized = false;   //  stop getHandle() and checkConnections() first.
    }
    B1LOG("waiting connections being released");
    const int32 sleepTime = 1000;   //  millisecond.
    const int32 trys = 60;          //  1 min.
    for (int32 i = 0; i < trys; ++i) {
        {
            B1AutoLock al(*_lock);
            SAFE_CONTAINER_FOREACH(_handles, itr) {
                if (itr->second != true) {
                    itr->first->finalize();
                    _handles.erase(itr);
                }
            }
            if (_handles.empty()) {
                break;
            }
        }
        B1Thread::sleep(sleepTime);
    }
    B1LOG("all connection released");
}

void B1MariaDBConnectionPool::checkConnections()
{
    size_t missingCount = 0;
    {
        B1AutoLock al(*_lock);
        if (_initialized != true) {
            return;
        }
        SAFE_CONTAINER_FOREACH(_handles, itr) {
            if (itr->second != true) {
                if (itr->first->isOpened() != true) {
                    itr->first->finalize();
                    _handles.erase(itr);
                }
            }
        }
        missingCount = _connectionCount > _handles.size() ? _connectionCount - _handles.size() : 0;   //  keep target count(even if re-creation failed before).
    }
    for (size_t i = 0; i < missingCount; ++i) {
        auto newHandle = createNewHandle();     //  out of lock: connecting may take long time.
        if (newHandle == nullptr) {
            B1LOG("unable to re-create new handle: index[%u], total[%u]", static_cast<uint32>(i), static_cast<uint32>(missingCount));
            return;                             //  retry at next checkConnections().
        }
        B1AutoLock al(*_lock);
        if (_initialized != true || _handles.size() >= _connectionCount) {  //  finalized or already filled while connecting.
            newHandle->finalize();
            return;
        }
        _handles.insert(std::make_pair(newHandle, false));
    }
}

int32 B1MariaDBConnectionPool::currentConnections() const
{
    B1AutoLock al(*_lock);
    return static_cast<int32>(_handles.size());
}

//bool B1MariaDBConnectionPool::increaseConnections(int32 count)
//{
//    B1AutoLock al(*_lock);
//}
//
//void B1MariaDBConnectionPool::decreaseConnections(int32 count)
//{
//    B1AutoLock al(*_lock);
//}

auto B1MariaDBConnectionPool::getHandle(int32 trys) -> std::shared_ptr<B1MariaDBHandle>
{
    const int32 sleepTime = 1000;   //  millisecond.
    for (int32 i = 0; i < trys; ++i) {
        {
            B1AutoLock al(*_lock);
            if (_initialized != true) {
                return std::shared_ptr<B1MariaDBHandle>();
            }
            for (auto& handlePair : _handles) {
                if (handlePair.second != true && handlePair.first->isOpened()) {
                    handlePair.second = true;
                    return handlePair.first;
                }
            }
        }
        B1Thread::sleep(sleepTime);
    }
    return std::shared_ptr<B1MariaDBHandle>();
}

void B1MariaDBConnectionPool::releaseHandle(std::shared_ptr<B1MariaDBHandle> handle)
{
    B1AutoLock al(*_lock);
    auto itr = _handles.find(handle);
    if (itr != _handles.end() && _initialized) {
        itr->second = false;
        return;
    }
    if (itr != _handles.end()) {
        _handles.erase(itr);    //  finalizing.
    }
    handle->finalize();
}
