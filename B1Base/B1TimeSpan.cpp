//
// B1TimeSpan.cpp
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Base.h"
#include "B1Archive.h"
#include "B1String.h"
#include "B1TimeSpan.h"

using namespace BnD;

void B1TimeSpan::archiveTo(B1Archive* archive) const
{
    writeDataToArchive("TotalSeconds", totalSeconds(), archive);
}

void B1TimeSpan::unarchiveFrom(const B1Archive& archive)
{
    int32 totalSeconds = 0;
    readDataFromArchive("TotalSeconds", archive, &totalSeconds);
    set(totalSeconds);
}

B1TimeSpan& B1TimeSpan::operator+=(const B1TimeSpan& ts)
{
    set(totalSeconds() + ts.totalSeconds());
    return *this;
}

B1TimeSpan& B1TimeSpan::operator-=(const B1TimeSpan& ts)
{
    set(totalSeconds() - ts.totalSeconds());
    return *this;
}

bool B1TimeSpan::operator==(const B1TimeSpan& ts) const
{
    return _days == ts._days &&
        _hours == ts._hours &&
        _minutes == ts._minutes &&
        _seconds == ts._seconds;
}

void B1TimeSpan::set(int32 days, int32 hours, int32 minutes, int32 seconds)
{
    assert(-24 < hours && hours < 24);
    assert(-60 < minutes && minutes < 60);
    assert(-60 < seconds && seconds < 60);
    _days    = days;
    _hours   = hours;
    _minutes = minutes;
    _seconds = seconds;
}

void B1TimeSpan::set(int32 totalSeconds)
{
    _days    = totalSeconds / (60 * 60 * 24);
    _hours   = (totalSeconds / (60 * 60)) % 24;
    _minutes = (totalSeconds / 60) % 60;
    _seconds = totalSeconds % 60;
}

void B1TimeSpan::set(const B1TimeSpan& ts)
{
    _days    = ts._days;
    _hours   = ts._hours;
    _minutes = ts._minutes;
    _seconds = ts._seconds;
}

B1String B1TimeSpan::toString() const
{
    B1String s;
    s.format("%d-%d:%d:%d (total %d seconds)", _days, _hours, _minutes, _seconds, totalSeconds());
    return s;
}
