//
// B1ArrayBufferServerSession.h
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1NETWORK_ARRAY_BUFFER_SERVER_SESSION_H
#define _B1NETWORK_ARRAY_BUFFER_SERVER_SESSION_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1ArrayBufferReadWriteImpl.h>
#include <B1Network/B1BaseServerSession.h>

namespace BnD {
    class B1ArrayBufferServerSession : protected B1ArrayBufferReadWriteImplListener
                                     , public B1BaseServerSession {
    public:
        B1ArrayBufferServerSession(B1ServerSocket* serverSocket, B1BaseServerSessionListener* listener);
        virtual ~B1ArrayBufferServerSession();
    protected:
        enum CONSTS {
            CONSTS_BUFFER_SIZE = 1024 * 4,
        };
    protected:
        virtual size_t arrayBufferSize() { return CONSTS_BUFFER_SIZE; }
    protected:  //  B1ArrayBufferReadWriteImplListener
        void onReadFailed(int32 reason) final;
        void onWriteFailed(int32 reason) final;
    protected:  //  B1BaseServerSession
        B1BaseReadWriteImpl* createReadWriteImpl() final;
    protected:
        B1ArrayBufferReadWriteImpl* readWriteImpl() const;
        void writeData(const uint8* data, size_t size) const;
        bool writeData(const std::vector<uint8>& data) const;
    };
}   //  !BnD

#endif  // !_B1NETWORK_ARRAY_BUFFER_SERVER_SESSION_H
