//
// B1PathInfo.cpp
//
// Library: B1Path
// Package: B1Path
// Module:  B1Path
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Path.h"
#include "B1PathInfo.h"

#include <boost/foreach.hpp>

using namespace BnD;

B1PathInfo::B1PathInfo()
    : _zoneIDs()
    , _junctionIndex()
    , _currentIndex(0)
{
}

B1PathInfo::~B1PathInfo()
{
}

void B1PathInfo::archiveTo(B1Archive *archive) const
{
    writeDataToArchive("ZoneIDs", _zoneIDs, archive);
    writeDataToArchive("JunctionIndex", std::vector<uint32>(_junctionIndex.begin(), _junctionIndex.end()), archive);
    writeDataToArchive("CurrentIndex", _currentIndex, archive);
}

void B1PathInfo::unarchiveFrom(const B1Archive &archive)
{
    readDataFromArchive("ZoneIDs", archive, &_zoneIDs);
    std::vector<uint32> junctionIndex;
    readDataFromArchive("JunctionIndex", archive, &junctionIndex);
    _junctionIndex.insert(junctionIndex.begin(), junctionIndex.end());
    readDataFromArchive("CurrentIndex", archive, &_currentIndex);
}

void B1PathInfo::addZoneID(int32 zoneID)
{
    _zoneIDs.push_back(zoneID);
}

void B1PathInfo::addZoneIDs(const std::vector<int32> &zoneIDs)
{
    _zoneIDs.insert(_zoneIDs.end(), zoneIDs.begin(), zoneIDs.end());
}

int32 B1PathInfo::currentZoneID() const
{
    int32 zoneID = 0;
    getZoneID(&zoneID, 0);
    return zoneID;
}

bool B1PathInfo::getJunctionZoneIDsIfJunction(std::vector<int32>* result) const
{
    auto itr = _junctionIndex.find(_currentIndex);
    if (itr == _junctionIndex.end())
        return false;
    for (++itr; itr != _junctionIndex.end(); ++itr) {
        if (_zoneIDs.size() > *itr) {
            result->push_back(_zoneIDs[*itr]);
        }
    }
    return true;
}

void B1PathInfo::removeAfterCurrentIndex(uint32 offset)
{
    if (static_cast<int32>(_zoneIDs.size()) > static_cast<int32>(_currentIndex + offset)) {
        _zoneIDs.erase(_zoneIDs.begin() + _currentIndex + offset, _zoneIDs.end());
    }
}

void B1PathInfo::getCurrentZoneIDs(std::vector<int32> *zoneIDs, uint32 indexCount) const
{
    zoneIDs->reserve(indexCount);
    for (uint32 i = _currentIndex; i < _currentIndex + indexCount; ++i) {
        if (_zoneIDs.size() <= i) {
            break;
        }
        zoneIDs->push_back(_zoneIDs[i]);
    }
}

bool B1PathInfo::getZoneID(int32 *zoneID, int32 offset) const
{
    int32 index = _currentIndex + offset;
    if (index < 0 || static_cast<int32>(_zoneIDs.size()) <= index)
        return false;
    *zoneID = _zoneIDs[index];
    return true;
}

std::vector<int32> B1PathInfo::getAllNextZoneIDs(uint32 offset) const
{
    if (static_cast<int32>(_zoneIDs.size()) > static_cast<int32>(_currentIndex + offset)) {
        return std::vector<int32>(_zoneIDs.begin() + _currentIndex + offset, _zoneIDs.end());
    }
    return std::vector<int32>();
}

bool B1PathInfo::isEndZone() const
{
    int32 size = static_cast<int32>(_zoneIDs.size());
    return (size == _currentIndex + 1);
}

int32 B1PathInfo::lastJunctionZoneID() const
{
    if (_junctionIndex.empty())
        return -1;
    BOOST_REVERSE_FOREACH(uint32 junctionIndex, _junctionIndex) {
        if (static_cast<uint32>(_zoneIDs.size()) > junctionIndex)
            return _zoneIDs[junctionIndex];
    }
    return -1;
}

int32 B1PathInfo::beginZoneID() const
{
    return _zoneIDs.empty() ? -1 : _zoneIDs.front();
}

int32 B1PathInfo::endZoneID(uint32 offset) const
{
    int32 size = static_cast<int32>(_zoneIDs.size());
    int32 index = size - offset - 1;
    return (index > -1 && size > index) ? _zoneIDs[index] : -1;
}

B1String B1PathInfo::toString() const
{
    B1String string = "B1PathInfo: zoneIDs";
    for (int32 zoneID : _zoneIDs) {
        string.appendf("[%d]", zoneID);
    }
    string.append(", junction");
    for (uint32 junctionIndex : _junctionIndex) {
        string.appendf(" index[%u]zoneID[%d]", junctionIndex, static_cast<uint32>(_zoneIDs.size()) > junctionIndex ? _zoneIDs[junctionIndex] : -1);
    }
    string.appendf(", current index[%u]zoneID[%d]", _currentIndex, static_cast<int32>(_zoneIDs.size()) > _currentIndex ? _zoneIDs[_currentIndex] : -1);
    return string;
}
