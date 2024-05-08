//
// B1BaseSession.h
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1NETWORK_BASE_SESSION_H
#define _B1NETWORK_BASE_SESSION_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    class B1BaseReadWriteImpl;
    class B1BaseSocket;
    class B1BaseSocketImpl;
    class B1BaseSession {
    public:
        B1BaseSession(B1BaseSocket* baseSocket);
        virtual ~B1BaseSession();
    protected:
        B1BaseSession(const B1BaseSession& baseSession, bool firstConnectedProcess);
    public:
        enum STATUS {
            STATUS_NONE = -1,
            STATUS_DISCONNECTED = 0,
            STATUS_CONNECTING,
            STATUS_CONNECTED,
            STATUS_DISCONNECTING,
        };
    private:
        bool _firstConnectedProcess;
        B1BaseSocket* _baseSocket;
        int32 _disconnectingReason;
        STATUS _lastConnectionStatus;
        STATUS _connectionStatus;
    protected:
        int32 _sessionHandleID;
        std::shared_ptr<B1BaseReadWriteImpl> _readWriteImpl;
    private:
        void setSessionStatusDisconnected();
    protected:
        virtual B1BaseReadWriteImpl* createReadWriteImpl() = 0;
        virtual void implOnConnect() {}                 //  called from network thread(DO NOT BLOCK). Connect not completed yet(do not call send, etc, ...).
        virtual void implOnDisconnected(int32 reason) {}//  called from network thread(DO NOT BLOCK). Disconnect already completed(do not call send, etc, ...).
        virtual void implDisconnect();
        virtual void implOnConnectionStatusChanged(STATUS previousStatus, STATUS newStatus) {}
        virtual void implProcessConnected(bool firstConnectedProcess) {}    //  called from worker thread(if blocked, all sessions are also affected).
        virtual void implProcessDisconnected() {}                           //  called from worker thread(if blocked, all sessions are also affected).
    protected:
        void setSessionStatusConnecting();
        void setSessionStatusConnected();
        void setSessionStatusDisconnecting(int32 reason = 0);
        bool beginRead();
        B1BaseSocket* baseSocket() const { return _baseSocket; }
        B1BaseReadWriteImpl* readWriteImpl() const { return _readWriteImpl.get(); }
    public:
        bool initializeSession();
        void finalizeSession();
        void cleanupSession();
        bool isDisconnecting() const;
        bool isDisconnected() const;
        bool isConnecting() const;
        bool isConnected() const;
        bool isFirstConnectedProcessComplete() const { return _firstConnectedProcess != true; }
        void disconnect();
        void processConnected();
        void processDisconnected();
        void setSessionHandleID(int32 id) { _sessionHandleID = id; }
        int32 sessionHandleID() const { return _sessionHandleID; }
        B1String peerAddress() const;
        uint16 peerPort() const;
        uint16 localPort() const;
        int32 currentConnectionStatus() const;
        void setDisconnectedIfSessionClosed();
    };
}   //  !BnD

#endif  // !_B1NETWORK_BASE_SESSION_H
