//
// B1RedisDirectClient.h
//
// Library: B1Redic
// Package: B1Redic
// Module:  B1Redic
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1REDIC_REDIS_DIRECT_CLIENT_H
#define _B1REDIC_REDIS_DIRECT_CLIENT_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1BaseClient.h>

#include <list>
#include <map>
#include <set>

namespace BnD {
    class B1RedisDirectClientListener;
    class B1RedisDirectClientMessageListener;
    class B1RedisDirectPacketMaker;
    class B1RedisDirectClientReadSession;
    class B1RedisDirectClientSubscribeSession;
    class B1RedisDirectClientWriteSession;
    class B1RedisDirectClient : protected B1BaseClient {
    public:
        enum ALIVE {
            ALIVE_DEFAULT_MAX_COUNT = 2,
            ALIVE_DEFAULT_INTERVAL = 1000,
        };
    public:
        B1RedisDirectClient(B1RedisDirectClientMessageListener* messageListener, B1RedisDirectClientListener* listener = NULL,
                          int32 maxAliveCount = ALIVE_DEFAULT_MAX_COUNT, int32 aliveInterval = ALIVE_DEFAULT_INTERVAL);
        virtual ~B1RedisDirectClient();
    protected:
        enum CONSTS {
            CONSTS_CONNECT_TIMEOUT = 1000 * 10,
            CONSTS_CONNECTED_CHECK_INTERVAL = 200,
        };
        enum SESSION_TYPE {
            SESSION_TYPE_WRITE = 0,
            SESSION_TYPE_READ,      //  blocked session.
            SESSION_TYPE_SUBSCRIBE, //  only (P)SUBSCRIBE / (P)UNSUBSCRIBE / PING / QUIT allowed in this context.
        };
    protected:
        const int32 _maxAliveCount;
        const int32 _aliveInterval;
        B1RedisDirectClientListener* _listener;
        B1RedisDirectClientMessageListener* _messageListener;
        std::shared_ptr<B1RedisDirectPacketMaker> _packetMaker;
        bool _startWritingNow;
        int32 _db;
        B1String _pollingKey;
        B1RedisDirectClientReadSession* _readSession;
        B1RedisDirectClientSubscribeSession* _subscribeSession;
        B1RedisDirectClientWriteSession* _writeSession;
    protected:
        virtual B1BaseSessionManager* createSessionManager() override;
        B1BaseClientSession* createSession(B1ClientSocket* clientSocket, void* param) final;
    public:
        int32 maxAliveCount() const { return _maxAliveCount; }
        int32 aliveInterval() const { return _aliveInterval; }
        B1RedisDirectClientListener* listener() { return _listener; }
        B1RedisDirectClientMessageListener* messageListener() { return _messageListener; }
        B1RedisDirectPacketMaker* packetMaker() const { return _packetMaker.get(); }
        int32 db() const { return _db; }
        const B1String& pollingKey() const { return _pollingKey; }
        bool initialize(const B1String& address, uint16 port, int32 db,
                        bool useSubscribe = true, B1String&& pollingKey = "", bool startWritingNow = true);    //  disable polling if pollingKey is empty.
        void finalize();
        bool startPolling();
        void startWriting();
        void setAdjustSeconds(int32 seconds);
        bool isReadSessionConnected() const;
        bool isSubscribeSessionConnected() const;
        bool isWriteSessionConnected() const;
        bool exists(const B1String& key);
        bool subscribe(const std::set<B1String>& channels);
        bool hmget(const B1String& key, const std::vector<B1String>& fields, std::vector<B1String>* out);
        B1String hmget(const B1String& key, const B1String& field);
        bool hgetall(const B1String& key, std::map<B1String, B1String>* out);
        bool smembers(const B1String& key, std::list<B1String>* out);
        bool incr(const B1String& key, int64* result);
        bool time(uint64* seconds, uint32* microseconds, bool ignoreAdjustSecond = false);
        void time(B1String* result, bool useTimeLength17 = false);
        void timeMMDDHHmmSSnnnn(B1String* result);
        bool shutdownRemoteRedis();
        bool blockedCommand(const std::vector<B1String>& args, std::list<B1String>* out = NULL);
        void blockedCommand(const std::vector<std::vector<B1String> >& cmds);
        bool publish(const B1String& channel, const B1String& message, bool isEssential);
        bool hmset(const B1String& key, const B1String& field, const B1String& value, bool isEssential);
        bool hmset(const B1String& key, const std::map<B1String, B1String>& fieldValues, bool isEssential);
        bool nonBlockedCommand(const std::vector<B1String>& args, bool isEssential);
        bool hdel(const B1String& key, const B1String& field);  //  Essential only.
        bool del(const B1String& key);                          //  Essential only.
        bool sadd(const B1String& key, const B1String& value);  //  Essential only.
        bool srem(const B1String& key, const B1String& value);  //  Essential only.
        bool rpush(const B1String& key, const B1String& value); //  Non-Essential only.
    };
}   //  !BnD

#endif  // !_B1REDIC_REDIS_DIRECT_CLIENT_H
