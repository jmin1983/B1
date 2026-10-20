//
// B1SECS2Server.h
//
// Library: B1SECS2
// Package: Server
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_SERVER_H
#define _B1SECS2_SERVER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1BaseServer.h>

namespace BnD {
    class B1SECS2DataManager;
    class B1SECS2Server : protected B1BaseServer {
    public:
        B1SECS2Server(uint16 deviceID);
        virtual ~B1SECS2Server();
    protected:
        const uint16 _deviceID;
        std::shared_ptr<B1SECS2DataManager> _serverSecs2DataManager;
    protected:
        virtual B1BaseServerSession* createSession(B1ServerSocket* serverSocket) override;
    protected:
        virtual bool implInitialize(int32 port);
        virtual void implFinalize();
        virtual std::shared_ptr<B1SECS2DataManager> createServerSECS2DataManager() = 0;
    public:
        bool initialize(int32 port);
        void finalize();
        const B1SECS2DataManager* serverSecs2DataManager() const { return _serverSecs2DataManager.get(); }
    };
}   //  !BnD

#endif  // !_B1SECS2_SERVER_H
