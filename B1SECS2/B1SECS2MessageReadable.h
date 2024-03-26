//
// B1SECS2MessageReadable.h
//
// Library: B1SECS2
// Package: Messages
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_MESSAGE_READABLE_H
#define _B1SECS2_MESSAGE_READABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2Data.h>
#include <B1SECS2/B1SECS2DataManager.h>
#include <B1SECS2/B1SECS2Message.h>

namespace BnD {
    class B1SECS2MessageReadable :public B1SECS2Message {
    public:
        B1SECS2MessageReadable(bool wait, uint8 stream, uint8 function, B1String&& messageName, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager);
        virtual ~B1SECS2MessageReadable();
    private:
        const uint8* _data;
        uint32 _dataLength;
        uint32 _dataPos;
        const B1SECS2DataManager* _dataManager;
    protected:
        virtual bool implReadData() { return false; }
    protected:
        bool readDummyList(uint32 expectedSize);
        bool readSECS2Data(B1SECS2Data* data);
        bool readSECS2Data(B1SECS2DataBase* data);
        bool readSECS2DataList(std::shared_ptr<B1SECS2DataLIST> list, std::list<SPB1SECS2Data>* result);
        std::shared_ptr<B1SECS2Data> readUnknownSECS2Data();
        const B1SECS2DataManager* dataManager() const { return _dataManager; }
    public:
        bool readData();
    };
}   //  !BnD

#endif  // !_B1SECS2_MESSAGE_READABLE_H
