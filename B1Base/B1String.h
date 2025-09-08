//
// B1String.h
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1BASE_STRING_H
#define _B1BASE_STRING_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <cstring>
#include <string>
#include <vector>

namespace BnD {
    class B1String {
    public:
        B1String() {}
        B1String(const B1String& s) = delete;
        B1String(const std::string& s) = delete;
        B1String(B1String& s) noexcept : _string(std::move(s._string)) {}
        B1String(B1String&& s) noexcept : _string(std::move(s._string)) {}
        B1String(std::string&& s) noexcept : _string(std::move(s)) {}
        B1String(uint32 count, char ch) : _string(static_cast<uint32>(count), ch) {}
        B1String(const char* cs);
        virtual ~B1String() {}
    protected:
        std::string _string;
    public:
        static const uint32 NPOS = 0xFFFFFFFF; // std::string::npos;
    private:
        void vformat(std::string* result, const std::string& format, va_list args);
    public:
        bool isEmpty() const { return _string.empty(); }
        uint32 length() const { return static_cast<uint32>(_string.length()); }
        void clear() { _string.clear(); }
        void assign(const B1String& s) { _string.assign(s._string); }
        void append(const B1String& s) { _string.append(s._string); }
        void append(const B1String& s, size_t count) { _string.append(s._string.c_str(), count); }
        int32 compare(const B1String& s) const { return _string.compare(s._string); }
        int32 compare(uint32 index, uint32 length, const B1String& s) const;
        bool caseInsensitiveCompare(const B1String& s) const;
        void makeUpper();
        void makeLower();
        void trim();
        void trimLeft();
        void trimRight();
        void format(const char* format, ...);
        void format(const B1String* format, ...);
        static B1String formatAs(const char* format, ...);
        void appendf(const char* format, ...);
        void appendf(const B1String* format, ...);
        void appendvf(const char* format, va_list args);
        uint32 find(const B1String& s, uint32 index = 0) const;
        uint32 findFirstOf(const B1String& s, uint32 index = NPOS) const;
        uint32 findLastOf(const B1String& s, uint32 index = NPOS) const;
        uint32 caseInsensitiveFind(const B1String& s) const;
        B1String substring(uint32 index, uint32 length = NPOS) const;
        B1String copy() const;
        void resize(uint32 size);
        const char* cString() const { return _string.c_str(); }
        void from(const char* cs) { _string = cs; }
        void from(std::string&& s) { _string = s; }
        const std::string& to_string() const { return _string; }
        int8  toInt8() const;
        uint8  toUint8() const;
        int16 toInt16() const;
        uint16 toUint16() const;
        int32 toInt32() const;
        uint32 toUint32() const;
        int64 toInt64() const;
        uint64 toUint64() const;
        float32 toFloat32() const;
        float64 toFloat64() const;

        template<class OutputIterator>
        OutputIterator toknize(const B1String& delimiter, OutputIterator out) const;

        B1String& operator=(const B1String& s) = delete;
        B1String& operator=(B1String&& s) noexcept { _string.assign(std::move(s._string)); return *this; }
        B1String& operator+=(const B1String& s) { _string.append(s._string); return *this; }
        B1String& operator+=(const char* cs) { _string += cs; return *this; }
        B1String& operator+=(char c) { _string += c; return *this; }
        const char& operator[](uint32 index) const { return _string[index]; }
        char& operator[](uint32 index) { return _string[index]; }
        bool operator==(const B1String& s) const { return _string == s._string; }
        bool operator!=(const B1String& s) const { return _string != s._string; }
        bool operator<=(const B1String& s) const { return _string <= s._string; }
        bool operator< (const B1String& s) const { return _string <  s._string; }
        bool operator>=(const B1String& s) const { return _string >= s._string; }
        bool operator> (const B1String& s) const { return _string >  s._string; }
        bool operator==(const char* cs) const { return _string == cs; }
        bool operator!=(const char* cs) const { return _string != cs; }
        bool operator<=(const char* cs) const { return _string <= cs; }
        bool operator< (const char* cs) const { return _string <  cs; }
        bool operator>=(const char* cs) const { return _string >= cs; }
        bool operator> (const char* cs) const { return _string >  cs; }
        friend bool operator==(const char* cs, const B1String& s) { return cs == s._string; }
        friend bool operator!=(const char* cs, const B1String& s) { return cs != s._string; }
        friend bool operator<=(const char* cs, const B1String& s) { return cs <= s._string; }
        friend bool operator< (const char* cs, const B1String& s) { return cs < s._string; }
        friend bool operator>=(const char* cs, const B1String& s) { return cs >= s._string; }
        friend bool operator> (const char* cs, const B1String& s) { return cs > s._string; }
    };

    template<class OutputIterator>
    OutputIterator B1String::toknize(const B1String& delimiter, OutputIterator out) const
    {
        std::vector<char> temp(_string.length() + 1, 0);
        memcpy(&temp[0], _string.c_str(), _string.length());
        char* tok = strtok(&temp[0], delimiter.cString());
        while (tok != NULL) {
            *out++ = tok;
            tok = strtok(NULL, delimiter.cString());
        }
        return out;
    }

    inline B1String operator+(const B1String& s1, const B1String& s2) { return B1String(std::move(s1.to_string() + s2.to_string())); }
    inline B1String operator+(const char* cs, const B1String& s) { return B1String(std::move(std::string(cs) + s.to_string())); }
    inline B1String operator+(const B1String& s, const char* cs) { return B1String(std::move(s.to_string() + std::string(cs))); }

}   //  !BnD

template <>
struct std::hash<BnD::B1String> {
    size_t operator()(const BnD::B1String& s) const { return std::hash<std::string>()(s.to_string()); }
};

#endif  // !_B1BASE_STRING_H
