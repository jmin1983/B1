//
// B1Time.h
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1BASE_TIME_H
#define _B1BASE_TIME_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Base/B1Object.h>

#if defined(_WIN32)
#include <ctime>    // time_t, struct tm
#endif

namespace BnD {
    class B1Time : public B1Object {
    public:
        B1Time();
        B1Time(const B1Time &t);
        B1Time(int32 year, int32 month, int32 day, int32 hour = 0, int32 minute = 0, int32 second = 0);
        explicit B1Time(time_t64 t);
        explicit B1Time(const struct tm &lt);
        virtual ~B1Time();
        enum {
            MIN_YEAR = 1970,
            MAX_YEAR = 2033,
        };
        enum DayOfWeek {
            SUN = 0, MON, TUE, WED, THU, FRI, SAT   //  DayOfWeek index is same with struct tm::tm_wday.
        };
    private:
        union {
            struct {
#if defined(_IS_SYSTEM_BIG_ENDIAN)
                uint32 year : 6;    // 0 ~ 63
                uint32 month : 4;    // 1 ~ 12
                uint32 day : 5;    // 1 ~ 31
                uint32 hour : 5;    // 0 ~ 23
                uint32 minute : 6;    // 0 ~ 59
                uint32 second : 6;    // 0 ~ 59
#else
                uint32 second : 6;    // 0 ~ 59
                uint32 minute : 6;    // 0 ~ 59
                uint32 hour : 5;    // 0 ~ 23
                uint32 day : 5;    // 1 ~ 31
                uint32 month : 4;    // 1 ~ 12
                uint32 year : 6;    // 0 ~ 63
#endif
            } _field;
            uint32 _value;
        };
        static int64 _adjustCurrentSeconds;
        static int32 _adjustCurrentMicroSeconds;
    protected:
        bool localTime(time_t64 t, struct tm *lt) const;
        time_t64 mkTime(struct tm *lt) const;
    protected:
        void archiveTo(B1Archive *archive) const final;
        void unarchiveFrom(const B1Archive &archive) final;
    public:
        bool isValid() const;
        bool isValid(int32 year, int32 month, int32 day, int32 hour, int32 minute, int32 second) const;
        int32 year() const { return MIN_YEAR + _field.year; }
        int32 month() const { return _field.month; }
        int32 day() const { return _field.day; }
        int32 hour() const { return _field.hour; }
        int32 minute() const { return _field.minute; }
        int32 second() const { return _field.second; }
        DayOfWeek dayOfWeek() const;

        void makeInvalid();
        void set(int32 year, int32 month, int32 day, int32 hour = 0, int32 minute = 0, int32 second = 0);
        void set(const B1Time &t) { _value = t._value; }
        time_t64 to_time_t() const;
        struct tm to_struct_tm() const;
        void from_time_t(time_t64 t);
        void from_struct_tm(const struct tm &t);
        uint32 toUint32() const { return _value; }
        void fromUint32(uint32 i) { _value = i; }

        B1String toString() const;

        static struct tm to_struct_tm(const B1Time& time);
        static bool isLeapYear(int32 year);
        static int32 daysInMonth(int32 year, int32 month);
        static B1Time currentTime();
        static B1String currentTimeInSeconds(bool pretty = false);
        static B1String currentTimeInMilliseconds(bool pretty = false, int64* currentSecond = NULL, int32* currentMicroSecond = NULL);
        static B1String currentTimeInMicroseconds(int64* currentSecond = NULL, int32* currentMicroSecond = NULL);   //  pretty only.
        static B1String timeInMillisecond(int64 second, int32 microSecond, bool pretty = false);
        static B1String timeInMicrosecond(int64 second, int32 microSecond); //  pretty only.
        static B1String timeInSecond(int64 second, bool pretty = false);
        static bool getCurrentTime(int64* second, int32* microSecond);
        static bool getSystemTime(int64* second, int32* microSecond);
        static void setAdjustCurrentTime(int64 targetSeconds, int32 targetMicroSeconds);
        static void setAdjustCurrentTimeAsIs(int64 adjustCurrentSeconds, int32 adjustCurrentMilliseconds);
        static void getAdjustCurrentTimeAsIs(int64* adjustCurrentSeconds, int32* adjustCurrentMilliseconds);
        static void setCurrentTime(const B1Time &t);    //  not supported in Windows.
        static bool setCurrentTime(const B1String& millisecondsString); //  not supported in Windows.
    public:
        B1Time& operator=(const B1Time &t) { set(t); return *this; }
        bool operator==(const B1Time &t) const { return _value == t._value; }
        bool operator!=(const B1Time &t) const { return _value != t._value; }
        bool operator<=(const B1Time &t) const { return _value <= t._value; }
        bool operator< (const B1Time &t) const { return _value < t._value; }
        bool operator>=(const B1Time &t) const { return _value >= t._value; }
        bool operator> (const B1Time &t) const { return _value > t._value; }
    };

    bool isSameDate(const B1Time &t1, const B1Time &t2);
    bool isSameHour(const B1Time &t1, const B1Time &t2);
    bool isSameMinute(const B1Time &t1, const B1Time &t2);
}   //  !BnD

#endif  // !_B1BASE_TIME_H
