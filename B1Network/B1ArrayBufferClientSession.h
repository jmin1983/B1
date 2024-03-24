//
// B1ArrayBufferClientSession.h
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1NETWORK_ARRAY_BUFFER_CLIENT_SESSION_H
#define _B1NETWORK_ARRAY_BUFFER_CLIENT_SESSION_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1ArrayBufferReadWriteImpl.h>
#include <B1Network/B1BaseClientSession.h>

namespace BnD {
    class B1ArrayBufferClientSession : protected B1ArrayBufferReadWriteImplListener
                                     , public B1BaseClientSession {
    public:
        B1ArrayBufferClientSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* listener);
        virtual ~B1ArrayBufferClientSession();
    protected:
        enum CONSTS {
            CONSTS_BUFFER_SIZE = 1024 * 4,
        };
    protected:  //  B1ArrayBufferReadWriteImplListener
        void onReadFailed(int32 reason) final;
        void onWriteFailed(int32 reason) final;
    protected:  //  B1BaseClientSession
        B1BaseReadWriteImpl* createReadWriteImpl(B1BaseSocket* baseSocket) final;
    protected:
        B1ArrayBufferReadWriteImpl* readWriteImpl() const;
        void writeData(const uint8* data, size_t size) const;
        bool writeData(const std::vector<uint8>& data) const;
    };
}   //  !BnD

#endif  // !_B1NETWORK_ARRAY_BUFFER_CLIENT_SESSION_H
