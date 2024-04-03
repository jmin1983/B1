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

using namespace BnD;

B1MariaDBConnectionPool::B1MariaDBConnectionPool()
    : _address()
    , _dbName()
    , _user()
    , _password()
    , _port(0)
    , _useSSL(false)
    , _lock()
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
    if (_lock) {
        return false;
    }
    _address = std::move(address);
    _port = port;
    _dbName = std::move(dbName);
    _user = std::move(user);
    _password = std::move(password);
    _useSSL = useSSL;
    _lock.reset(new B1Lock());

    for (uint32 i = 0; i < connectionCount; ++i) {
        auto newHandle = createNewHandle();
        if (newHandle == NULL) {
            B1LOG("unable to create new handle: index[%u], total[%u]", i, connectionCount);
            return false;
        }
        _handles.insert(std::make_pair(newHandle, false));
    }
    return true;
}

void B1MariaDBConnectionPool::finalize()
{
    if (_lock) {
        B1LOG("waiting connections being released");
        const int32 sleepTime = 1000;   //  millisecond.
        const int32 trys = 60;          //  1 min.
        for (int32 i = 0; i < trys; ++i) {
            {
                B1AutoLock al(*_lock);
                for (auto itr = _handles.cbegin(); itr != _handles.end();) {
                    if (itr->second != true) {
                        itr->first->finalize();
                        itr = _handles.erase(itr);
                    }
                    else {
                        ++itr;
                    }
                }
                if (_handles.empty()) {
                    break;
                }
            }
            B1Thread::sleep(sleepTime);
        }
        B1LOG("all connection released");
        _lock.reset();
    }
}

void B1MariaDBConnectionPool::checkConnections()
{
    int32 deadHandleCount = 0;
    {
        B1AutoLock al(*_lock);
        for (auto itr = _handles.cbegin(); itr != _handles.end();) {
            if (itr->second != true) {
                if (itr->first->isOpened() != true) {
                    ++deadHandleCount;
                    itr->first->finalize();
                    itr = _handles.erase(itr);
                }
            }
            else {
                ++itr;
            }
        }
    }
    if (deadHandleCount > 0) {
        B1AutoLock al(*_lock);
        for (uint32 i = 0; i < deadHandleCount; ++i) {
            auto newHandle = createNewHandle();
            if (newHandle == NULL) {
                B1LOG("unable to re-create new handle: index[%u], total[%u]", i, deadHandleCount);
                return;
            }
            _handles.insert(std::make_pair(newHandle, false));
        }
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
    if (itr != _handles.end()) {
        itr->second = false;
    }
    else {
        handle->finalize();
    }
    handle.reset();
}
