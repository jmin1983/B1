//
// B1WebSocketReadWriteImpl.h
//
// Library: B1Http
// Package: Http
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
    class B1WebSocketMessage;
    class B1WebSocketReadWriteImplListener : public B1BaseReadWriteImplListener {
    public:
        virtual bool onWebSocketReadComplete(B1String&& text) { return true; }              //  return false if there are no more data to read.
        virtual bool onWebSocketReadComplete(std::vector<uint8>&& data) { return true; }    //  return false if there are no more data to read.
        virtual void onWebSocketWriteComplete(size_t dataSize) {}
    };

    class B1WebSocketReadWriteImpl : public B1BaseReadWriteImpl {
    public:
        B1WebSocketReadWriteImpl(B1WebSocketReadWriteImplListener* listener);
        virtual ~B1WebSocketReadWriteImpl();
    protected:
        class Writer : public B1Looper {
        public:
            Writer();
        protected:
            enum CONSTS {
                CONSTS_DEFAULT_TEXT_BUNCH_HINT = 1024 * 512,
            };
        protected:
            struct Data {
                enum TYPE {
                    TYPE_TEXT = 0,
                    TYPE_BINARY,
                };
                Data(TYPE type) : _type(type) {}
                TYPE _type;
                bool isTextData() const { return TYPE_TEXT == _type; }
                bool isBinaryData() const { return TYPE_BINARY == _type; }
            };
            struct TextData : Data {
                TextData(B1String&& text) : Data(TYPE_TEXT), _text(std::move(text)) {}
                B1String _text;
            };
            struct BinaryData : Data {
                BinaryData(std::vector<uint8>&& binary) : Data(TYPE_BINARY) { _binary.swap(binary); }
                std::vector<uint8> _binary;
            };
        protected:
            B1WebSocketReadWriteImpl* _owner;
            bool _readyToWrite;
            uint32 _textBunchHint;
            mutable B1Lock _lock;
            std::list<std::shared_ptr<Data> > _data;
        protected:
            void implLooperFunc() final;
        public:
            void setOwner(B1WebSocketReadWriteImpl* owner) { _owner = owner; }
            void setTextBunchHint(uint32 value) { _textBunchHint = value; }
            void addWriteBinary(std::vector<uint8>&& data);
            void addWriteText(B1String&& text);
            void writeComplete() { _readyToWrite = true; }
        } _writer;
    protected:
        std::shared_ptr<B1WebSocketMessage> _webSocketMessage;
    protected:
        virtual void implWriteBinary(std::vector<uint8>&& data);
        virtual void implWriteText(B1String&& data);
    protected:
        virtual B1BaseSocketImpl* createBaseSocketImpl() override;
        virtual bool implRead() override;
        virtual bool implOnReadComplete(size_t receivedBytes) override; //  return false if there are no more data to read.
    protected:
        B1WebSocketReadWriteImplListener* listener() const;
        void writeBinary(std::vector<uint8>&& data);
        void writeText(B1String&& text);
        void writeBinaryComplete(std::vector<uint8>* data, const boost::system::error_code& error, size_t transferredBytes);
        void writeTextComplete(std::string* text, const boost::system::error_code& error, size_t transferredBytes);
    public:
        void addWriteBinary(std::vector<uint8>&& data);
        void addWriteText(B1String&& text);
        void setTextBunchHint(uint32 value);
        class B1WebSocketImpl* webSocketImpl() const;
    };
}   //  !BnD

#endif  // !_B1HTTP_WEB_SOCKET_READ_WRITE_IMPL_H
