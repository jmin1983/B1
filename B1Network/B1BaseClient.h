//
// B1BaseClient.h
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1NETWORK_BASE_CLIENT_H
#define _B1NETWORK_BASE_CLIENT_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1BaseComponent.h>
#include <B1Network/B1BaseClientSessionListener.h>

namespace BnD {
    class B1BaseClientSession;
    class B1ClientSocket;
    class B1BaseClient : protected B1BaseClientSessionListener
                       , public B1BaseComponent {
    public:
        B1BaseClient();
        virtual ~B1BaseClient();
    private:
        bool _waiterForConnectSync;
    protected:
        virtual B1BaseClientSession* createSession(B1ClientSocket* clientSocket, void* param) = 0;
    protected:  //  B1BaseClientSessionListener
        virtual void onClientSessionConnectResult(B1ClientSocket* clientSocket, bool isConnected, int32 reason) override;
        virtual void onClientSessionClosed(B1ClientSocket* clientSocket) override;
    public:
        B1ClientSocket* connect(B1String&& address, uint16 port, void* param = NULL, int32 requestedHandleID = -1);        //  return NULL if failed.
        B1ClientSocket* connectSync(B1String&& address, uint16 port, void* param = NULL, int32 requestedHandleID = -1);    //  return NULL if failed.
        void disconnect(B1ClientSocket* handle);
        void disconnectByHandleID(int32 handleID);
        void closeSession(B1ClientSocket* handle);
        bool isConnected(B1ClientSocket* handle) const;
        int32 currentConnectionStatus(B1ClientSocket* handle) const;
    };
}   //  !BnD

#endif  // !_B1NETWORK_BASE_CLIENT_H
