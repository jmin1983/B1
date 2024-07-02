//
// B1RedisDirectClientReadSession.h
//
// Library: B1Redic
// Package: B1Redic
// Module:  B1Redic
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1REDIC_REDIS_DIRECT_CLIENT_READ_SESSION_H
#define _B1REDIC_REDIS_DIRECT_CLIENT_READ_SESSION_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif
 
#include <B1Redic/B1RedisDirectClientSession.h>

#include <map>
#include <set>

namespace BnD {
    class B1Condition;
    class B1Lock;
    class B1RedisDirectClientReadSession : public B1RedisDirectClientSession {  //  blocking session.
    public:
        B1RedisDirectClientReadSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* listener, B1RedisDirectClient* owner);
        virtual ~B1RedisDirectClientReadSession();
    protected:
        enum CONSTS {
            CONSTS_WAIT_DATA_TIME_OUT = 3000,
        };
    private:
        bool _inited;
        std::list<B1String> _recvedBuffer;
    protected:
        int32 _adjustSeconds;
        std::shared_ptr<B1Condition> _wait;
        std::shared_ptr<B1Lock> _lock;
    protected:  //  B1RedisDirectClientSession
        void implOnRecvRedisSimpleStrings(const B1String& string) final;
        void implOnRecvRedisErrors(const B1String& message) final;
        void implOnRecvRedisIntegers(const B1String& int64String) final;
        void implOnRecvRedisBulkStrings(const B1String& string) final;
        void implOnRecvRedisArrays(const std::list<B1String>& strings) final;
    protected:
        void implOnDisconnected(int32 reason) final;
        virtual void implProcessConnected(bool firstConnectedProcess) override;
    protected:
        void implCheckAlive() final;
    protected:    //  locked functions.
        bool setWaitData();                                 //  return false if already_waiting.
        bool waitData(std::list<B1String>* data = NULL);    //  return false if timed_out.
        bool scan(const B1String& cursor, const B1String& pattern, std::set<B1String>* out, int32 count);
    public:
        bool exists(const B1String& key);
        bool hmget(const B1String& key, const std::vector<B1String>& fields, std::vector<B1String>* out);
        B1String hmget(const B1String& key, const B1String& field);
        bool hgetall(const B1String& key, std::map<B1String, B1String>* out);
        bool scan(const B1String& pattern, std::set<B1String>* out, int32 count = -1);
        bool smembers(const B1String& key, std::list<B1String>* out);
        bool incr(const B1String& key, int64* result);
        bool time(uint64* seconds, uint32* microseconds, bool ignoreAdjustSecond = false);
        void time(B1String* result, bool useTimeLength17);
        void timeMMDDHHmmSSnnnn(B1String* result);
        bool shutdownRemoteRedis();
        bool blockedCommand(const std::vector<B1String>& args, std::list<B1String>* out = NULL);
        void blockedCommand(const std::vector<std::vector<B1String> >& cmds);

        void setAdjustSeconds(int32 seconds) { _adjustSeconds = seconds; }
    };
}   //  !BnD

#endif  // !_B1REDIC_REDIS_DIRECT_CLIENT_READ_SESSION_H
