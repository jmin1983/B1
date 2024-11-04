//
// B1Time.cpp
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Base.h"
#include "B1Time.h"
#include "B1String.h"

#if defined(_WIN32)
#include <winsock.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif

using namespace BnD;

int64 B1Time::_adjustCurrentSeconds = 0;
int32 B1Time::_adjustCurrentMicroSeconds = 0;

B1Time::B1Time()
{
    makeInvalid();
}

B1Time::B1Time(const B1Time &t)
{
    set(t);
}

B1Time::B1Time(int32 year, int32 month, int32 day, int32 hour, int32 minute, int32 second)
{
    set(year, month, day, hour, minute, second);
}

B1Time::B1Time(time_t64 t)
{
    from_time_t(t);
}

B1Time::B1Time(const struct tm &lt)
{
    from_struct_tm(lt);
}

B1Time::~B1Time()
{
}

bool B1Time::localTime(time_t64 t, struct tm *lt) const
{
#if defined(_WIN32)
    struct tm* p = ::_localtime64(&t);
    return (p) ? (*lt = *p, true) : false;
#else
    time_t tt = static_cast<time_t>(t);
    return (0 != ::localtime_r(&tt, lt));
#endif
}

time_t64 B1Time::mkTime(struct tm *lt) const
{
#if defined(_WIN32)
    return ::_mktime64(lt);
#else
    return static_cast<time_t64>(::mktime(lt));
#endif
}

void B1Time::archiveTo(B1Archive *archive) const
{
    writeDataToArchive("Time", to_time_t(), archive);
}

void B1Time::unarchiveFrom(const B1Archive &archive)
{
    int64 t = 0;
    readDataFromArchive("Time", archive, &t);
    from_time_t(t);
}

bool B1Time::isValid() const
{
    return isValid(year(), month(), day(), hour(), minute(), second());
}

bool B1Time::isValid(int32 year, int32 month, int32 day, int32 hour, int32 minute, int32 second) const
{
    return MIN_YEAR <= year && year <= MAX_YEAR &&
           1 <= month && month <= 12 &&
           1 <= day && day <= B1Time::daysInMonth(year, month) &&
           0 <= hour && hour <= 23 &&
           0 <= minute && minute <= 59 &&
           0 <= second && second <= 59;
}

B1Time::DayOfWeek B1Time::dayOfWeek() const
{
    return static_cast<DayOfWeek>(to_struct_tm().tm_wday);
}

void B1Time::makeInvalid()
{
    _field.year = 0;
    _field.month = 13;
    _field.day = 0;
    _field.hour = 25;
    _field.minute = 61;
    _field.second = 61;
}

void B1Time::set(int32 year, int32 month, int32 day, int32 hour, int32 minute, int32 second)
{
    if (isValid(year, month, day, hour, minute, second)) {
        _field.year = year - MIN_YEAR;
        _field.month = month;
        _field.day = day;
        _field.hour = hour;
        _field.minute = minute;
        _field.second = second;
    }
    else {
        makeInvalid();
    }
}

time_t64 B1Time::to_time_t() const
{
    struct tm lt = to_struct_tm(*this);
    return mkTime(&lt);
}

struct tm B1Time::to_struct_tm() const
{
    struct tm lt = to_struct_tm(*this);
    mkTime(&lt);
    return lt;
}

void B1Time::from_time_t(time_t64 t)
{
    struct tm lt;
    if (localTime(t, &lt)) {
        from_struct_tm(lt);
    }
    else {
        makeInvalid();
    }
}

void B1Time::from_struct_tm(const struct tm& t)
{
    set(t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
        t.tm_hour, t.tm_min, t.tm_sec);
}

B1String B1Time::toString() const
{
    return B1String::formatAs("%04d-%02d-%02d_%02d:%02d:%02d", year(), month(), day(), hour(), minute(), second());
}

struct tm B1Time::to_struct_tm(const B1Time& time)
{
    struct tm t;
    t.tm_year = time.year() - 1900;
    t.tm_mon = time.month() - 1;
    t.tm_mday = time.day();
    t.tm_hour = time.hour();
    t.tm_min = time.minute();
    t.tm_sec = time.second();
    t.tm_isdst = -1;
    return t;
}

bool B1Time::isLeapYear(int32 year)
{
    return (((year & 3) == 0 && (year % 100) != 0) || (year % 400) == 0);
}

int32 B1Time::daysInMonth(int32 year, int32 month)
{
    assert(0 < month && month < 13);
    const int32 s_monthDays[13] = {
        0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
    };
    int32 days = s_monthDays[month] - s_monthDays[month - 1];
    return (isLeapYear(year) && month == 2) ? days + 1 : days;
}

B1Time B1Time::currentTime()
{
    time_t64 t = static_cast<time_t64>(time(0));
    if (_adjustCurrentSeconds != 0) {
        t = static_cast<time_t64>(t + _adjustCurrentSeconds);
    }
    return B1Time(t);
}

#if defined(_WIN32)
int gettimeofday(struct timeval* tv, struct timezone* tz = NULL)
{
    const uint64 delta = 11644473600000000Ui64;
    if (NULL != tv) {
        FILETIME fileTime;
        GetSystemTimeAsFileTime(&fileTime);
        uint64 temp = 0;
        temp |= fileTime.dwHighDateTime;
        temp <<= 32;
        temp |= fileTime.dwLowDateTime;
        temp /= 10;     //  convert into microseconds.
        temp -= delta;  //  converting file time to unix epoch.
        tv->tv_sec = (long)(temp / 1000000UL);
        tv->tv_usec = (long)(temp  % 1000000UL);
    }
    return 0;
}
#endif

B1String B1Time::currentTimeInSeconds(bool pretty)
{
    int64 second = 0;
    int32 microSecond = 0;
    if (getCurrentTime(&second, &microSecond) != true) {
        return B1String();
    }
    return timeInSecond(second, pretty);
}

B1String B1Time::currentTimeInMilliseconds(bool pretty, int64* currentSecond, int32* currentMicroSecond)
{
    int64 second = 0;
    int32 microSecond = 0;
    if (getCurrentTime(&second, &microSecond) != true) {
        return B1String();
    }
    if (currentSecond) {
        *currentSecond = second;
    }
    if (currentMicroSecond) {
        *currentMicroSecond = microSecond;
    }
    return timeInMillisecond(second, microSecond, pretty);
}

B1String B1Time::currentTimeInMicroseconds(int64* currentSecond, int32* currentMicroSecond)
{
    int64 second = 0;
    int32 microSecond = 0;
    if (getCurrentTime(&second, &microSecond) != true) {
        return B1String();
    }
    if (currentSecond) {
        *currentSecond = second;
    }
    if (currentMicroSecond) {
        *currentMicroSecond = microSecond;
    }
    return timeInMicrosecond(second, microSecond);
}

B1String B1Time::timeInMillisecond(int64 second, int32 microSecond, bool pretty)
{
    time_t second_ = static_cast<time_t>(second);
    tm* crntTime = localtime(static_cast<const time_t*>(&second_));
    if (NULL == crntTime) {
        assert(false);
        return B1String();
    }
    if (pretty) {
        return B1String::formatAs("%04d-%02d-%02d_%02d:%02d:%02d.%03d",
                                  crntTime->tm_year + 1900, crntTime->tm_mon + 1, crntTime->tm_mday,
                                  crntTime->tm_hour, crntTime->tm_min, crntTime->tm_sec,
                                  microSecond / 1000);
    }
    else {
        return B1String::formatAs("%04d%02d%02d%02d%02d%02d%02d",
                                  crntTime->tm_year + 1900, crntTime->tm_mon + 1, crntTime->tm_mday,
                                  crntTime->tm_hour, crntTime->tm_min, crntTime->tm_sec,
                                  microSecond / 10000);
    }
}

B1String B1Time::timeInMicrosecond(int64 second, int32 microSecond)
{
    time_t second_ = static_cast<time_t>(second);
    tm* crntTime = localtime(static_cast<const time_t*>(&second_));
    if (NULL == crntTime) {
        assert(false);
        return B1String();
    }
    return B1String::formatAs("%04d-%02d-%02d_%02d:%02d:%02d.%06d",
                                crntTime->tm_year + 1900, crntTime->tm_mon + 1, crntTime->tm_mday,
                                crntTime->tm_hour, crntTime->tm_min, crntTime->tm_sec, microSecond);
}

B1String B1Time::timeInSecond(int64 second, bool pretty)
{
    time_t second_ = static_cast<time_t>(second);
    tm* crntTime = localtime(static_cast<const time_t*>(&second_));
    if (NULL == crntTime) {
        assert(false);
        return B1String();
    }
    if (pretty) {
        return B1String::formatAs("%04d-%02d-%02d_%02d:%02d:%02d",
                                  crntTime->tm_year + 1900, crntTime->tm_mon + 1, crntTime->tm_mday,
                                  crntTime->tm_hour, crntTime->tm_min, crntTime->tm_sec);
    }
    else {
        return B1String::formatAs("%04d%02d%02d%02d%02d%02d",
                                  crntTime->tm_year + 1900, crntTime->tm_mon + 1, crntTime->tm_mday,
                                  crntTime->tm_hour, crntTime->tm_min, crntTime->tm_sec);
    }
}

B1String B1Time::getCurrentTimeZone()
{
#if defined(_WIN32)
    _tzset();
    return _tzname[0];
#else
    tzset();
    return tzname[0];
#endif
}

bool B1Time::getCurrentTime(int64* second, int32* microSecond)
{
    timeval crntTimeVal;
    if (gettimeofday(&crntTimeVal, NULL) < 0) {
        return false;
    }
    if (_adjustCurrentSeconds != 0 || _adjustCurrentMicroSeconds != 0) {
        int64 newSecond = crntTimeVal.tv_sec + _adjustCurrentSeconds;
        int32 newMicroSecond = crntTimeVal.tv_usec + _adjustCurrentMicroSeconds;
        if (newMicroSecond >= 1000000) {
            newMicroSecond -= 1000000;
            newSecond++;
        }
        if (newSecond < 0 || newMicroSecond < 0) {
            assert(false);
            return false;
        }
        *second = newSecond;
        if (microSecond) {
            *microSecond = newMicroSecond;
        }
    }
    else {
        *second = crntTimeVal.tv_sec;
        if (microSecond) {
            *microSecond = crntTimeVal.tv_usec;
        }
    }
    return true;
}

bool B1Time::getSystemTime(int64* second, int32* microSecond)
{
    timeval crntTimeVal;
    if (gettimeofday(&crntTimeVal, NULL) < 0) {
        return false;
    }
    *second = crntTimeVal.tv_sec;
    if (microSecond) {
        *microSecond = crntTimeVal.tv_usec;
    }
    return true;
}

void B1Time::setAdjustCurrentTime(int64 targetSeconds, int32 targetMicroSeconds)
{
    timeval crntTimeVal;
    if (gettimeofday(&crntTimeVal, NULL) < 0) {
        assert(false);
        return;
    }
    if (crntTimeVal.tv_usec > targetMicroSeconds) {
        _adjustCurrentSeconds = targetSeconds - crntTimeVal.tv_sec - 1;
        _adjustCurrentMicroSeconds = targetMicroSeconds - crntTimeVal.tv_usec + 1000000;
    }
    else {
        _adjustCurrentSeconds = targetSeconds - crntTimeVal.tv_sec;
        _adjustCurrentMicroSeconds = targetMicroSeconds - crntTimeVal.tv_usec;
    }
}

void B1Time::setCurrentTime(const B1Time &t)
{
#if defined(_WIN32)
    assert(false);  //  not supported.
#else
    assert(t.isValid());
    struct timeval tv;
    tv.tv_sec = t.to_time_t();
    tv.tv_usec = 0;
    settimeofday(&tv, 0/*&tz*/);
#endif
}

bool B1Time::setCurrentTime(const B1String& millisecondsString)
{
    if (millisecondsString.length() != 16) {
        assert(false);
        return false;
    }
    int32 year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0, csecond = 0;
    try { year =    millisecondsString.substring(0, 4).toInt32(); } catch (...) { return false; }
    try { month =   millisecondsString.substring(4, 2).toInt32(); } catch (...) { return false; }
    try { day =     millisecondsString.substring(6, 2).toInt32(); } catch (...) { return false; }
    try { hour =    millisecondsString.substring(8, 2).toInt32(); } catch (...) { return false; }
    try { minute =  millisecondsString.substring(10, 2).toInt32(); } catch (...) { return false; }
    try { second =  millisecondsString.substring(12, 2).toInt32(); } catch (...) { return false; }
    try { csecond = millisecondsString.substring(14, 2).toInt32(); } catch (...) { return false; }

    B1Time t(year, month, day, hour, minute, second);
    if (t.isValid() != true) {
        assert(false);
        return false;
    }
#if defined(_WIN32)
    return true;
#else
    struct timeval tv;
    tv.tv_sec = t.to_time_t();
    tv.tv_usec = csecond * 10000;
    int result = settimeofday(&tv, 0/*&tz*/);
    return 0 == result;
#endif
}

void B1Time::setCurrentTimeZone(const B1String& timezone)
{
#if defined(_WIN32)
    return;
#else
    setenv("TZ", timezone.cString(), 1);
    tzset();
#endif
}

bool BnD::isSameDate(const B1Time& t1, const B1Time& t2)
{
    return t1.year() == t2.year() &&
           t1.month() == t2.month() &&
           t1.day() == t2.day();
}

bool BnD::isSameHour(const B1Time& t1, const B1Time& t2)
{
    return (isSameDate(t1, t2) && t1.hour() == t2.hour());
}

bool BnD::isSameMinute(const B1Time& t1, const B1Time& t2)
{
    return (isSameHour(t1, t2) && t1.minute() == t2.minute());
}
