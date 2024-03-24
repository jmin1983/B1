//
// B1WebSocketReadWriteImpl.h
//
// Library: B1Http
// Package: B1Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1HTTP_WEB_SOCKET_READ_WRITE_IMPL_H
#define _B1HTTP_WEB_SOCKET_READ_WRITE_IMPL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Base/B1Looper.h>
#include <B1Base/B1Lock.h>
#include <B1Network/B1BaseReadWriteImpl.h>

#include <list>

namespace boost {
    namespace system {
        class error_code;
    };
};

namespace BnD {
    class B1WebSocketImpl;
    class B1WebSocketMessage;
    class B1WebSocketReadWriteImplListener : public B1BaseReadWriteImplListener {
    public:
        virtual bool onWebSocketReadComplete(const B1String& text) { return true; }             //  return false if there are no more data to read.
        virtual bool onWebSocketReadComplete(const std::vector<uint8>& data) { return true; }   //  return false if there are no more data to read.
        virtual void onWebSocketWriteComplete(size_t dataSize) {}
    };

    class B1WebSocketReadWriteImpl : public B1BaseReadWriteImpl {
    public:
        B1WebSocketReadWriteImpl(B1BaseSocket* baseSocket, B1WebSocketReadWriteImplListener* listener);
        virtual ~B1WebSocketReadWriteImpl();
    private:
        class Writer : public B1Looper {
        public:
            Writer() : _owner(NULL), _readyToWrite(true) {}
        protected:
            B1WebSocketReadWriteImpl* _owner;
            bool _readyToWrite;
            B1Lock _lock;
            std::list<std::vector<uint8> > _binaryData;
            std::list<B1String> _textData;
        protected:
            void implLooperFunc() final;
        public:
            void setOwner(B1WebSocketReadWriteImpl* owner) { _owner = owner; }
            void addWriteBinary(std::vector<uint8>&& data);
            void addWriteText(B1String&& text);
            void writeComplete() { _readyToWrite = true; }
        } _writer;
    protected:
        B1WebSocketImpl* _webSocketImpl;
        std::shared_ptr<B1WebSocketMessage> _webSocketMessage;
    private:
        void writeBinaryComplete(std::vector<uint8>* data, const boost::system::error_code& error, size_t transferredBytes);
        void writeTextComplete(std::string* text, const boost::system::error_code& error, size_t transferredBytes);
    protected:
        bool implRead() final;
        bool implOnReadComplete(size_t receivedBytes) final;    //  return false if there are no more data to read.
    protected:
        B1WebSocketReadWriteImplListener* listener() const;
        void writeBinary(std::vector<uint8>* data);
        void writeText(const B1String& text);
    public:
        void setWebSocketImpl(B1WebSocketImpl* webSocketImpl);
        void addWriteBinary(std::vector<uint8>&& data);
        void addWriteText(B1String&& text);
    };
}   //  !BnD

#endif  // !_B1HTTP_WEB_SOCKET_READ_WRITE_IMPL_H
