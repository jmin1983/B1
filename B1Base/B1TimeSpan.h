//
// B1TimeSpan.h
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1BASE_TIME_SPAN_H
#define _B1BASE_TIME_SPAN_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Base/B1Object.h>
#include <B1Base/B1Time.h>

namespace BnD {
    class B1TimeSpan : public B1Object {
    public:
        B1TimeSpan() { set(0, 0, 0, 0); }
        B1TimeSpan(int32 days, int32 hours, int32 minutes, int32 seconds) { set(days, hours, minutes, seconds); }
        explicit B1TimeSpan(int32 totalSeconds) { set(totalSeconds); }
        B1TimeSpan(const B1TimeSpan& ts) { set(ts); }
        virtual ~B1TimeSpan() {}
    private:
        int32 _days;
        int32 _hours;
        int32 _minutes;
        int32 _seconds;
    protected:
        void archiveTo(B1Archive* archive) const final;
        void unarchiveFrom(const B1Archive& archive) final;
    public:
        B1TimeSpan& operator=(const B1TimeSpan& ts) { set(ts); return *this; }
        B1TimeSpan& operator+=(const B1TimeSpan& ts);
        B1TimeSpan& operator-=(const B1TimeSpan& ts);
        B1TimeSpan operator+(const B1TimeSpan& ts) const { return B1TimeSpan(*this) += ts; }
        B1TimeSpan operator-(const B1TimeSpan& ts) const { return B1TimeSpan(*this) -= ts; }
        bool operator==(const B1TimeSpan& ts) const;
        bool operator!=(const B1TimeSpan& ts) const { return !operator==(ts); }
        bool operator< (const B1TimeSpan& ts) const { return totalSeconds() < ts.totalSeconds(); }
        bool operator<=(const B1TimeSpan& ts) const { return totalSeconds() <= ts.totalSeconds(); }
        bool operator> (const B1TimeSpan& ts) const { return totalSeconds() > ts.totalSeconds(); }
        bool operator>=(const B1TimeSpan& ts) const { return totalSeconds() >= ts.totalSeconds(); }
    public:
        int32 days() const { return _days; }
        int32 hours() const { return _hours; }
        int32 minutes() const { return _minutes; }
        int32 seconds() const { return _seconds; }
        int32 totalHours() const { return _days * 24 + _hours; }
        int32 totalMinutes() const { return totalHours() * 60 + _minutes; }
        int32 totalSeconds() const { return totalMinutes() * 60 + _seconds; }
        void set(int32 days, int32 hours, int32 minutes, int32 seconds);
        void set(int32 totalSeconds);
        void set(const B1TimeSpan& ts);
        B1String toString() const;
    };

    inline B1TimeSpan operator-(const B1Time& t1, const B1Time& t2) {
        return B1TimeSpan(static_cast<int32>(t1.to_time_t() - t2.to_time_t()));
    }
    inline B1Time& operator+=(B1Time& t, const B1TimeSpan& ts) {
        t.from_time_t(static_cast<int32>(t.to_time_t()) + ts.totalSeconds());
        return t;
    }
    inline B1Time& operator-=(B1Time& t, const B1TimeSpan& ts) {
        t.from_time_t(static_cast<int32>(t.to_time_t()) - ts.totalSeconds());
        return t;
    }
    inline B1Time operator+(const B1Time& t, const B1TimeSpan& ts) {
        B1Time rt(t);
        return rt += ts;
    }
    inline B1Time operator-(const B1Time& t, const B1TimeSpan& ts) {
        B1Time rt(t);
        return rt -= ts;
    }
    inline B1Time operator+(const B1TimeSpan& ts, const B1Time& t) { return t + ts; }
}   //  !BnD

#endif  // _B1BASE_TIME_SPAN_H
