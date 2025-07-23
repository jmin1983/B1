//
// B1SECS2MessageWritable.h
//
// Library: B1SECS2
// Package: Messages
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_MESSAGE_WRITABLE_H
#define _B1SECS2_MESSAGE_WRITABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2Data.h>
#include <B1SECS2/B1SECS2DataManager.h>
#include <B1SECS2/B1SECS2Message.h>

namespace BnD {
    class B1SECS2Data;
    class B1SECS2MessageWritable : public B1SECS2Message {
    public:
        B1SECS2MessageWritable(bool wait, uint8 stream, uint8 function, B1String&& messageName);
        virtual ~B1SECS2MessageWritable();
    private:
        std::vector<std::vector<uint8> > _items;
    private:
        void appendToLogStringDecreaseDepthRecursive(int32* depth, std::list<int32>* remainDepths, B1String* log);
    protected:
        void implToString(B1String* string) final {}
    protected:
        virtual bool implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data) { return false; }
    protected:
        void appendToLogString(const std::list<SPB1SECS2Data>& data, B1String* log);
        void addList(size_t count, std::list<SPB1SECS2Data>* result);
        void addData(const SPB1SECS2Data& data, std::list<SPB1SECS2Data>* result);
        void addData(const B1SECS2DataBase& data, std::list<SPB1SECS2Data>* result);
        void addData(const std::list<SPB1SECS2Data>& data, std::list<SPB1SECS2Data>* result);
    public:
        bool writeData();
        bool wait() const { return _wait; }
        const std::vector<std::vector<uint8> >& items() const { return _items; }
    };
}   //  !BnD

#endif  // !_B1SECS2_MESSAGE_WRITABLE_H
