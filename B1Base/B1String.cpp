//
// B1String.cpp
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Base.h"
#include "B1String.h"

#include <algorithm>
#include <stdarg.h>
#include <cctype>

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/scoped_array.hpp>

#if !defined(_WIN32)
#define _snprintf snprintf
#endif

using namespace BnD;

B1String::B1String(const char* cs)
{
    try {
        if (cs) {
            _string = std::move(std::string(cs));
        }
        else {
            _string = "";
        }
    }
    catch (...) {
        _string = "";
    }
}

int32 B1String::compare(uint32 index, uint32 length, const B1String& s) const
{
    return _string.compare(index, length, s._string);
}

bool B1String::caseInsensitiveCompare(const B1String& s) const
{
    if (length() != s.length()) {
        return false;
    }
    auto result = boost::ifind_first(_string, s._string);
    return (result) ? (_string.begin() == result.begin()) : false;
}

void B1String::makeUpper()
{
    boost::to_upper(_string);
}

void B1String::makeLower()
{
    boost::to_lower(_string);
}

void B1String::trim()
{
    boost::trim(_string);
}

void B1String::trimLeft()
{
    boost::trim_left(_string);
}

void B1String::trimRight()
{
    boost::trim_right(_string);
}

void B1String::vformat(std::string* result, const std::string& format, va_list args)
{
    static const std::string types("diouxXDOUeEfFgGaAcspn");

    result->clear();

    std::string tf;
    const uint32 TS_N = 256;
    char ts[TS_N];
    boost::scoped_array<char> tss;
    std::string::const_iterator si = format.begin();
    std::string::const_iterator t, i = si;
    while (true) {
        i = std::find(i, format.end(), '%');
        if (i == format.end()) {
            result->append(si, format.end());
            break;
        }

        if (*(i + 1) == '%') {
            result->append(si, i + 1);
            i += 2;
            si = i;
            continue;
        }

        t = std::find_first_of(i, format.end(), types.begin(), types.end());
        if (t == format.end()) {
            abort();
            return;
        }

        result->append(si, i);

        tf.assign(i, t + 1);
        int32 dist = static_cast<int32>(std::distance(i, t));
        if (*t == 'd' || *t == 'i' || *t == 'o' ||
            *t == 'u' || *t == 'x' || *t == 'X') {
            if (4 <= dist && *(t - 3) == 'I' && *(t - 2) == '6' && *(t - 1) == '4') {
#if !defined(_WIN32)
                tf.replace(tf.length() - 4, 3, "ll");
#endif
                _snprintf(ts, TS_N, tf.c_str(), va_arg(args, int64));
            }
            else if (3 <= dist && *(t - 2) == 'l' && *(t - 1) == 'l') {
#if defined(_WIN32)
                tf.replace(tf.length() - 3, 2, "I64");
#endif
                _snprintf(ts, TS_N, tf.c_str(), va_arg(args, int64));
            }
            else if (2 <= dist && *(t - 1) == 'l') {   // "%..ld"
                _snprintf(ts, TS_N, tf.c_str(), va_arg(args, long));
            }
            else {                                      // "%..d"
                _snprintf(ts, TS_N, tf.c_str(), va_arg(args, int));
            }
        }
        else if (*t == 'e' || *t == 'E' || *t == 'f' || *t == 'F' ||
            *t == 'g' || *t == 'G' || *t == 'a' || *t == 'A') {
            _snprintf(ts, TS_N, tf.c_str(), va_arg(args, double));
        }
        else if (*t == 'c') {
            _snprintf(ts, TS_N, tf.c_str(), va_arg(args, int));
        }
        else if (*t == 's') {
            char* s = va_arg(args, char*);
            size_t tssn = std::strlen(s) + 1;
            if (TS_N < tssn) {
                tss.reset(new char[tssn]);
                _snprintf(tss.get(), tssn, tf.c_str(), s);
            }
            else {
                _snprintf(ts, TS_N, tf.c_str(), s);
            }
        }
        else if (*t == 'p') {
            _snprintf(ts, TS_N, tf.c_str(), va_arg(args, void*));
        }
        if (tss.get()) {
            result->append(tss.get());
            tss.reset();
        }
        else {
            ts[TS_N - 1] = '\0';
            result->append(ts);
        }
        i = t + 1;
        si = i;
    }
}

void B1String::format(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vformat(&_string, format, args);
    va_end(args);
}

void B1String::format(const B1String* format, ...)
{
    va_list args;
    va_start(args, format);
    vformat(&_string, format->_string, args);
    va_end(args);
}

B1String B1String::formatAs(const char* format, ...)
{
    B1String result;
    va_list args;
    va_start(args, format);
    result.vformat(&result._string, format, args);
    va_end(args);
    return result;
}

void B1String::appendf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    std::string s;
    vformat(&s, format, args);
    va_end(args);

    _string += s;
}

void B1String::appendf(const B1String* format, ...)
{
    va_list args;
    va_start(args, format);
    std::string s;
    vformat(&s, format->_string, args);
    va_end(args);

    _string += s;
}

void B1String::appendvf(const char* format, va_list args)
{
    std::string s;
    vformat(&s, format, args);
    _string += s;
}

uint32 B1String::find(const B1String& s, uint32 index) const
{
    std::string::size_type i = _string.find(s._string, index);
    return (i == std::string::npos) ? B1String::NPOS : static_cast<uint32>(i);
}

uint32 B1String::findFirstOf(const B1String& s, uint32 index) const
{
    std::string::size_type i = _string.find_first_of(s._string, index);
    return (i == std::string::npos) ? B1String::NPOS : static_cast<uint32>(i);
}

uint32 B1String::findFirstNotOf(const B1String& s, uint32 index) const
{
    std::string::size_type i = _string.find_first_not_of(s._string, index);
    return (i == std::string::npos) ? B1String::NPOS : static_cast<uint32>(i);
}

uint32 B1String::findLastOf(const B1String& s, uint32 index) const
{
    std::string::size_type i = _string.find_last_of(s._string, index);
    return (i == std::string::npos) ? B1String::NPOS : static_cast<uint32>(i);
}

uint32 B1String::findLastNotOf(const B1String& s, uint32 index) const
{
    std::string::size_type i = _string.find_last_not_of(s._string, index);
    return (i == std::string::npos) ? B1String::NPOS : static_cast<uint32>(i);
}

uint32 B1String::caseInsensitiveFind(const B1String& s) const
{
    auto r = boost::ifind_first(_string, s._string);
    return (r) ? static_cast<uint32>(distance(_string.begin(), r.begin())) : NPOS;
}

uint32 B1String::reverseFind(const B1String& s, uint32 index) const
{
    std::string::size_type pos = (index == B1String::NPOS) ?
        std::string::npos : static_cast<std::string::size_type>(index);
    std::string::size_type i = _string.rfind(s._string, pos);
    return (i == std::string::npos) ? B1String::NPOS : static_cast<uint32>(i);
}

B1String B1String::substring(uint32 index, uint32 length) const
{
    return _string.substr(index, length);
}

B1String B1String::copy() const
{
    std::string string = _string;
    return std::move(string);
}

void B1String::resize(uint32 size)
{
    _string.resize(static_cast<std::string::size_type>(size));
}

int8 B1String::toInt8() const
{
    return boost::numeric_cast<int8>(boost::lexical_cast<int32>(_string));
}

uint8 B1String::toUint8() const
{
    return boost::numeric_cast<uint8>(boost::lexical_cast<uint32>(_string));
}

int16 B1String::toInt16() const
{
    return boost::lexical_cast<int16>(_string);
}

uint16 B1String::toUint16() const
{
    return boost::lexical_cast<uint16>(_string);
}

int32 B1String::toInt32() const
{
    return boost::lexical_cast<int32>(_string);
}

uint32 B1String::toUint32() const
{
    return boost::lexical_cast<uint32>(_string);
}

int64 B1String::toInt64() const
{
    return boost::lexical_cast<int64>(_string);
}

uint64 B1String::toUint64() const
{
    return boost::lexical_cast<uint64>(_string);
}

float32 B1String::toFloat32() const
{
    return boost::lexical_cast<float32>(_string);
}

float64 B1String::toFloat64() const
{
    return boost::lexical_cast<float64>(_string);
}
