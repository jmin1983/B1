//
// B1RedisDirectClient.cpp
//
// Library: B1Redic
// Package: B1Redic
// Module:  B1Redic
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Redic.h"
#include "B1RedisDirectClient.h"
#include "B1RedisDirectClientPollingSession.h"
#include "B1RedisDirectClientReadSession.h"
#include "B1RedisDirectClientSubscribeSession.h"
#include "B1RedisDirectClientWriteSession.h"
#include "B1RedisDirectPacketMaker.h"

#include <B1Base/B1TickUtil.h>
#include <B1Network/B1BaseSessionManager.h>

using namespace BnD;

B1RedisDirectClient::B1RedisDirectClient(B1RedisDirectClientMessageListener* messageListener, B1RedisDirectClientListener* listener,
                                         int32 maxAliveCount, int32 aliveInterval)
    : B1BaseClient()
    , _maxAliveCount(maxAliveCount)
    , _aliveInterval(aliveInterval)
    , _listener(listener)
    , _messageListener(messageListener)
    , _packetMaker(new B1RedisDirectPacketMaker())
    , _startWritingNow(false)
    , _db(0)
    , _pollingKey("")
    , _readSession(NULL)
    , _subscribeSession(NULL)
    , _writeSession(NULL)
{
}

B1RedisDirectClient::~B1RedisDirectClient()
{
}

B1BaseSessionManager* B1RedisDirectClient::createSessionManager()
{
    return new B1BaseSessionManager(CONSTS_CONNECTED_CHECK_INTERVAL);
}

B1BaseClientSession* B1RedisDirectClient::createSession(B1ClientSocket* clientSocket, void* param)
{
    B1BaseClientSession* newSession = NULL;
    switch (static_cast<int32>((intptr_t)param)) {
        default:
            assert(false);
        case SESSION_TYPE_WRITE:
            assert(_writeSession == NULL);
            _writeSession = new B1RedisDirectClientWriteSession(clientSocket, this, this, _startWritingNow);
            newSession = _writeSession;
            break;
        case SESSION_TYPE_READ:
            assert(_readSession == NULL);
            _readSession = _pollingKey.isEmpty() ? new B1RedisDirectClientReadSession(clientSocket, this, this) : new B1RedisDirectClientPollingSession(clientSocket, this, this);
            newSession = _readSession;
            break;
        case SESSION_TYPE_SUBSCRIBE:
            assert(_subscribeSession == NULL);
            _subscribeSession = new B1RedisDirectClientSubscribeSession(clientSocket, this, this);
            newSession = _subscribeSession;
            break;
    }
    return newSession;
}

bool B1RedisDirectClient::initialize(const B1String& address, uint16 port, int32 db,
                                     bool useSubscribe, B1String&& pollingKey, bool startWritingNow, const uint32 connectTimeout)
{
    if (_readSession != NULL || _subscribeSession != NULL || _writeSession != NULL) {
        assert(false);
        return false;
    }
    if (startup() != true) {
        return false;
    }
    _startWritingNow = startWritingNow;
    _db = db;
    _pollingKey = std::move(pollingKey);
    auto writeContext = B1BaseClient::connect(address.copy(), port, (void*)static_cast<intptr_t>(SESSION_TYPE_WRITE));
    if (NULL == writeContext) {
        _readSession = NULL;
        _subscribeSession = NULL;
        _writeSession = NULL;
        shutdown();
        return false;
    }
    auto readContext = B1BaseClient::connect(address.copy(), port, (void*)static_cast<intptr_t>(SESSION_TYPE_READ));
    if (NULL == readContext) {
        disconnect(writeContext);
        _readSession = NULL;
        _subscribeSession = NULL;
        _writeSession = NULL;
        shutdown();
        return false;
    }
    B1ClientSocket* subscribeContext = NULL;
    if (useSubscribe) {
        subscribeContext = B1BaseClient::connect(address.copy(), port, (void*)static_cast<intptr_t>(SESSION_TYPE_SUBSCRIBE));
        if (subscribeContext == NULL) {
            disconnect(readContext);
            disconnect(writeContext);
            _readSession = NULL;
            _subscribeSession = NULL;
            _writeSession = NULL;
            shutdown();
            return false;
        }
    }

    uint64 connectTimeoutTickBegin = B1TickUtil::currentTick();
    while (true) {
        if (isReadSessionConnected() && isWriteSessionConnected() && (useSubscribe != true || isSubscribeSessionConnected())) {
            break;
        }
        if (B1TickUtil::diffTick(connectTimeoutTickBegin, B1TickUtil::currentTick()) > connectTimeout) {
            disconnect(readContext);
            disconnect(writeContext);
            if (subscribeContext) {
                disconnect(subscribeContext);
            }
            _readSession = NULL;
            _subscribeSession = NULL;
            _writeSession = NULL;
            shutdown();
            return false;
        }
        B1Thread::sleep(1);
    }
    return true;
}

bool B1RedisDirectClient::initializeReadOnly(const B1String& address, uint16 port, int32 db, const uint32 connectTimeout)
{
    if (_readSession != NULL || _subscribeSession != NULL || _writeSession != NULL) {
        assert(false);
        return false;
    }
    if (startup() != true) {
        return false;
    }
    _db = db;
    auto readContext = B1BaseClient::connect(address.copy(), port, (void*)static_cast<intptr_t>(SESSION_TYPE_READ));
    if (NULL == readContext) {
        _readSession = NULL;
        _subscribeSession = NULL;
        _writeSession = NULL;
        shutdown();
        return false;
    }

    uint64 connectTimeoutTickBegin = B1TickUtil::currentTick();
    while (true) {
        if (isReadSessionConnected()) {
            break;
        }
        if (B1TickUtil::diffTick(connectTimeoutTickBegin, B1TickUtil::currentTick()) > connectTimeout) {
            disconnect(readContext);
            _readSession = NULL;
            _subscribeSession = NULL;
            _writeSession = NULL;
            shutdown();
            return false;
        }
        B1Thread::sleep(1);
    }
    return true;
}

void B1RedisDirectClient::finalize()
{
    _readSession = NULL;
    _subscribeSession = NULL;
    _writeSession = NULL;
    _db = 0;
    shutdown();
}

bool B1RedisDirectClient::startPolling()
{
    if (auto pollingSession = dynamic_cast<B1RedisDirectClientPollingSession*>(_readSession)) {
        pollingSession->startPolling();
        return true;
    }
    else {
        return false;
    }
}

void B1RedisDirectClient::startWriting()
{
    if (_writeSession) {
        _writeSession->startWriting();
    }
}

void B1RedisDirectClient::setAdjustSeconds(int32 seconds)
{
    if (_readSession) {
        _readSession->setAdjustSeconds(seconds);
    }
}

bool B1RedisDirectClient::isReadSessionConnected() const
{
    return _readSession ? _readSession->isRedisDirectSessionConnected() : false;
}

bool B1RedisDirectClient::isSubscribeSessionConnected() const
{
    return _subscribeSession ? _subscribeSession->isRedisDirectSessionConnected() : false;
}

bool B1RedisDirectClient::isWriteSessionConnected() const
{
    return _writeSession ? _writeSession->isRedisDirectSessionConnected() : false;
}

bool B1RedisDirectClient::exists(const B1String& key)
{
    return _readSession ? _readSession->exists(key) : false;
}

bool B1RedisDirectClient::subscribe(std::set<B1String>&& channels)
{
    if (NULL == _subscribeSession) {
        B1LOG("subscribe disabled in this client");
        return false;
    }
    return _subscribeSession->subscribe(std::move(channels));
}

bool B1RedisDirectClient::hmget(const B1String& key, const std::vector<B1String>& fields, std::vector<B1String>* out)
{
    return _readSession ? _readSession->hmget(key, fields, out) : false;
}

B1String B1RedisDirectClient::hmget(const B1String& key, const B1String& field)
{
    return _readSession ? _readSession->hmget(key, field) : "";
}

bool B1RedisDirectClient::hgetall(const B1String& key, std::map<B1String, B1String>* out)
{
    return _readSession ? _readSession->hgetall(key, out) : false;
}

bool B1RedisDirectClient::scan(const B1String& pattern, std::set<B1String>* out, int32 count)
{
    return _readSession ? _readSession->scan(pattern, out, count) : false;
}

bool B1RedisDirectClient::smembers(const B1String& key, std::list<B1String>* out)
{
    return _readSession ? _readSession->smembers(key, out) : false;
}

bool B1RedisDirectClient::incr(const B1String& key, int64* result)
{
    return _readSession ? _readSession->incr(key, result) : false;
}

bool B1RedisDirectClient::time(uint64* seconds, uint32* microseconds, bool ignoreAdjustSecond)
{
    return _readSession ? _readSession->time(seconds, microseconds, ignoreAdjustSecond) : false;
}

bool B1RedisDirectClient::shutdownRemoteRedis()
{
    return _readSession ? _readSession->shutdownRemoteRedis() : false;
}

bool B1RedisDirectClient::blockedCommand(const std::vector<B1String>& args, std::list<B1String>* out)
{
    return _readSession ? _readSession->blockedCommand(args, out) : false;
}

void B1RedisDirectClient::blockedCommand(const std::vector<std::vector<B1String> >& cmds)
{
    if (_readSession) {
        _readSession->blockedCommand(cmds);
    }
}

bool B1RedisDirectClient::publish(const B1String& channel, const B1String& message, bool isEssential)
{
    return _writeSession ? _writeSession->publish(channel, message, isEssential) : false;
}

bool B1RedisDirectClient::hmset(const B1String& key, const B1String& field, const B1String& value, bool isEssential)
{
    std::map<B1String, B1String> fieldValues;
    fieldValues.insert(std::make_pair(field.copy(), value.copy()));
    return hmset(key, fieldValues, isEssential);
}

bool B1RedisDirectClient::hmset(const B1String& key, const std::map<B1String, B1String>& fieldValues, bool isEssential)
{
    return _writeSession ? _writeSession->hmset(key, fieldValues, isEssential) : false;
}

bool B1RedisDirectClient::nonBlockedCommand(const std::vector<B1String>& args, bool isEssential)
{
    return _writeSession ? _writeSession->nonBlockedCommand(args, isEssential) : false;
}

bool B1RedisDirectClient::hdel(const B1String& key, const B1String& field)
{
    return _writeSession ? _writeSession->hdel(key, field) : false;
}

bool B1RedisDirectClient::del(const B1String& key)
{
    return _writeSession ? _writeSession->del(key) : false;
}

bool B1RedisDirectClient::sadd(const B1String& key, const B1String& value)
{
    return _writeSession ? _writeSession->sadd(key, value) : false;
}

bool B1RedisDirectClient::srem(const B1String& key, const B1String& value)
{
    return _writeSession ? _writeSession->srem(key, value) : false;
}

bool B1RedisDirectClient::rpush(const B1String& key, const B1String& value)
{
    return _writeSession ? _writeSession->rpush(key, value) : false;
}
