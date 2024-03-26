//
// B1SECS2Message.cpp
//
// Library: B1SECS2
// Package: Messages
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2Message.h"

using namespace BnD;

B1SECS2Message::B1SECS2Message(bool wait, uint8 stream, uint8 function, B1String&& messageName)
    : _wait(wait)
    , _stream(stream)
    , _function(function)
    , _messageName(std::move(messageName))
{
}

B1SECS2Message::~B1SECS2Message()
{
}

void B1SECS2Message::appendStringDepth(int32 depth, const B1String& text, B1String* string) const
{
    const static B1String s_gap = "  ";
    for (int32 i = 0; i < depth; ++i) {
        string->append(s_gap);
    }
    string->appendf("%s\n", text.cString());
}

void B1SECS2Message::appendStringDepthList(int32 depth, uint32 count, B1String* string) const
{
    const static B1String s_gap = "  ";
    for (int32 i = 0; i < depth; ++i) {
        string->append(s_gap);
    }
    string->appendf("<L %u\n", count);
}

void B1SECS2Message::appendStringDepth0(const B1String& text, B1String* string) const
{
    string->appendf("%s\n", text.cString());
}

void B1SECS2Message::appendStringDepth0List(size_t size, B1String* string) const
{
    string->appendf("<L %u\n", size);
}

void B1SECS2Message::appendStringDepth1(const B1String& text, B1String* string) const
{
    string->appendf("  %s\n", text.cString());
}

void B1SECS2Message::appendStringDepth1List(size_t size, B1String* string) const
{
    string->appendf("  <L %u\n", size);
}

void B1SECS2Message::appendStringDepth2(const B1String& text, B1String* string) const
{
    string->appendf("    %s\n", text.cString());
}

void B1SECS2Message::appendStringDepth2List(size_t size, B1String* string) const
{
    string->appendf("    <L %u\n", size);
}

void B1SECS2Message::appendStringDepth3(const B1String& text, B1String* string) const
{
    string->appendf("      %s\n", text.cString());
}

void B1SECS2Message::appendStringDepth3List(size_t size, B1String* string) const
{
    string->appendf("      <L %u\n", size);
}

void B1SECS2Message::appendStringDepth4(const B1String& text, B1String* string) const
{
    string->appendf("        %s\n", text.cString());
}

void B1SECS2Message::appendStringDepth5(const B1String& text, B1String* string) const
{
    string->appendf("          %s\n", text.cString());
}

B1String B1SECS2Message::toString()
{
    B1String string;
    appendStringDepth0(_messageName.cString(), &string);
    appendStringDepth0(fullName().cString(), &string);
    if (_wait)
        appendStringDepth0("W", &string);
    implToString(&string);
    return string;
}
