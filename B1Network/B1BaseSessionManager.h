//
// B1BaseSessionManager.h
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1NETWORK_BASE_SESSION_MANAGER_H
#define _B1NETWORK_BASE_SESSION_MANAGER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Base/B1Lock.h>
#include <B1Base/B1Looper.h>

#include <list>
#include <map>

namespace BnD {
    class B1BaseSession;
    class B1BaseSocket;
    class B1BaseSessionManager : protected B1Looper {
    public:
        B1BaseSessionManager(uint32 workingInerval = DEFAULT_WORKING_INTERVAL);
        virtual ~B1BaseSessionManager();
    protected:
        struct SessionData {
            SessionData(int32 handleID, std::shared_ptr<B1BaseSession> session, std::shared_ptr<B1BaseSocket> baseSocket)
                : _handleID(handleID), _session(session), _baseSocket(baseSocket) {}
            int32 _handleID;
            std::shared_ptr<B1BaseSession> _session;
            std::shared_ptr<B1BaseSocket> _baseSocket;
        };
    protected:
        enum {
            DEFAULT_WORKING_INTERVAL = 1000,
        };
    protected:
        uint32 _workingInerval;
        mutable B1Lock _sessionsLock;
        std::map<B1BaseSocket*, SessionData> _sessions; //  map<handle, session_data>.
        std::map<int32, B1BaseSocket*> _handleIDMap;    //  map<handle_id, handle>.
        std::list<SessionData> _reservedRemoveSessions;
    protected:
        virtual void onShuttingDown() {}
        virtual void onSessionAdded(int32 handleID, std::shared_ptr<B1BaseSession> session) {}
        virtual void onSessionModified(int32 handleID, std::shared_ptr<B1BaseSession> session) {}
        virtual void onSessionRemoved(int32 handleID) {}
    protected:
        void implLooperFunc() final;
    public:
        int32 addSession(std::shared_ptr<B1BaseSocket> baseSocket, std::shared_ptr<B1BaseSession> session, int32 requestedHandleID = -1);   //  return handleID. return negative if failed.
        bool modifySession(B1BaseSocket* baseSocket, std::shared_ptr<B1BaseSession> session);
        void reserveRemoveSession(B1BaseSocket* baseSocket);
        bool empty();
        bool startup();
        void shutdown();
        void disconnectAllSessions();
        int32 getHandleID(B1BaseSocket* baseSocket) const;
        uint32 sessionCount() const;
        std::map<B1BaseSocket*, SessionData> getAllSessions() const;
        std::shared_ptr<B1BaseSession> getBaseSession(B1BaseSocket* baseSocket) const;
        std::shared_ptr<B1BaseSession> getBaseSessionByHandleID(int32 handleID) const;
        template <typename T>
        std::shared_ptr<T> getSession(B1BaseSocket* baseSocket) const
        {
            B1AutoLock al(_sessionsLock);
            auto itr = _sessions.find(baseSocket);
            return itr != _sessions.end() ? std::dynamic_pointer_cast<T>(itr->second._session) : std::shared_ptr<T>();
        }
        template <typename T>
        std::shared_ptr<T> getSessionByHandleID(int32 handleID) const
        {
            B1BaseSocket* baseSocket = NULL;
            B1AutoLock al(_sessionsLock);
            {
                auto itr = _handleIDMap.find(handleID);
                if (itr != _handleIDMap.end()) {
                    baseSocket = itr->second;
                }
            }
            if (NULL == baseSocket) {
                return std::shared_ptr<T>();
            }
            auto itr = _sessions.find(baseSocket);
            return itr != _sessions.end() ? std::dynamic_pointer_cast<T>(itr->second._session) : std::shared_ptr<T>();
        }
        template <typename T>
        std::shared_ptr<T> getUniqueSession() const
        {
            B1AutoLock al(_sessionsLock);
            return _sessions.size() == 1 ? std::dynamic_pointer_cast<T>(_sessions.begin()->second._session) : std::shared_ptr<T>();
        }
    };
}   //  !BnD

#endif  // !_B1NETWORK_BASE_SESSION_MANAGER_H
