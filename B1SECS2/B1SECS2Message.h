//
// SECS2EquipmentConstantVariable.h
//
// Library: B1SECS2
// Package: Messages
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_MESSAGE_H
#define _B1SECS2_MESSAGE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    class B1SECS2Message {
    public:
        B1SECS2Message(bool wait, uint8 stream, uint8 function, B1String&& messageName);
        virtual ~B1SECS2Message();
    protected:
        const bool _wait;
        const uint8 _stream;
        const uint8 _function;
        const B1String _messageName;
    protected:
        virtual B1String fullName() = 0;
        virtual void implToString(B1String* string) {}
    protected:
        void appendStringDepth(int32 depth, const B1String& text, B1String* string) const;
        void appendStringDepthList(int32 depth, uint32 count, B1String* string) const;
        void appendStringDepthCloser(int32 depth, B1String* string) const { appendStringDepth(depth, ">", string); }
        void appendStringDepth0(const B1String& text, B1String* string) const;
        void appendStringDepth0List(size_t size, B1String* string) const;
        void appendStringDepth0Closer(B1String* string) const { appendStringDepth0(">", string); }
        void appendStringDepth1(const B1String& text, B1String* string) const;
        void appendStringDepth1List(size_t size, B1String* string) const;
        void appendStringDepth1Closer(B1String* string) const { appendStringDepth1(">", string); }
        void appendStringDepth2(const B1String& text, B1String* string) const;
        void appendStringDepth2List(size_t size, B1String* string) const;
        void appendStringDepth2Closer(B1String* string) const { appendStringDepth2(">", string); }
        void appendStringDepth3(const B1String& text, B1String* string) const;
        void appendStringDepth3List(size_t size, B1String* string) const;
        void appendStringDepth3Closer(B1String* string) const { appendStringDepth3(">", string); }
        void appendStringDepth4(const B1String& text, B1String* string) const;
        void appendStringDepth4Closer(B1String* string) const { appendStringDepth4(">", string); }
        void appendStringDepth5(const B1String& text, B1String* string) const;
        void appendStringDepth5Closer(B1String* string) const { appendStringDepth5(">", string); }
    public:
        uint8 stream() const { return _stream; }
        uint8 function() const { return _function; }
        B1String toString();
    };

    typedef std::shared_ptr<B1SECS2Message> SPB1SECS2Message;
}   //  !BnD

#endif  // !_B1SECS2_MESSAGE_H
