//
// B1HttpServerSessionManager.h
//
// Library: B1Http
// Package: B1Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1HTTP_HTTP_SERVER_SESSION_MANAGER_H
#define _B1HTTP_HTTP_SERVER_SESSION_MANAGER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1BaseSessionManager.h>

namespace BnD {
    class B1HttpServerSessionManager : public B1BaseSessionManager {
    public:
        B1HttpServerSessionManager();
        virtual ~B1HttpServerSessionManager();
    protected:
        mutable B1Lock _webSocketSessionsLock;
        std::map<int32, std::shared_ptr<B1BaseSession> > _webSocketSessions;    //  map<handle_id, session>
    protected:
        virtual void onShuttingDown() override;
        virtual void onSessionModified(int32 handleID, std::shared_ptr<B1BaseSession> session) override;
        virtual void onSessionRemoved(int32 handleID) override;
    public:
        std::map<int32, std::shared_ptr<B1BaseSession> > webSocketSessions() const;
        std::shared_ptr<B1BaseSession> webSocketSessionByHandleID(int32 handleID) const;
    };
}   //  !BnD

#endif  // !_B1HTTP_HTTP_SERVER_SESSION_MANAGER_H
