//
// B1PathInfo.h
//
// Library: B1Path
// Package: B1Path
// Module:  B1Path
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1PATH_INFO_H
#define _B1PATH_INFO_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Base/B1Object.h>

#include <set>

namespace BnD {
    class B1PathInfo : public B1Object {
    public:
        B1PathInfo();
        virtual ~B1PathInfo();
    protected:
        std::vector<int32> _zoneIDs;
        std::set<uint32> _junctionIndex;
        int32 _currentIndex;
    protected:
        virtual void archiveTo(B1Archive *archive) const override;
        virtual void unarchiveFrom(const B1Archive &archive) override;
    public:
        void addZoneID(int32 zoneID);
        void addZoneIDs(const std::vector<int32> &zoneIDs);
        void addJunctionIndex(uint32 index) { _junctionIndex.insert(index); }
        const std::vector<int32>& zoneIDs() const { return _zoneIDs; }
              std::vector<int32>& zoneIDs()       { return _zoneIDs; }
        const std::set<uint32>& junctionIndex() const { return _junctionIndex; }
              std::set<uint32>& junctionIndex()       { return _junctionIndex; }
        int32 currentIndex() const { return _currentIndex; }
        int32 currentZoneID() const;
        bool getJunctionZoneIDsIfJunction(std::vector<int32>* result) const;
        bool isJunction(uint32 index) const { return _junctionIndex.find(index) != _junctionIndex.end(); }
        void increaseCurrentIndex() { _currentIndex++; }
        void setCurrentIndex(int32 value) { _currentIndex = value; }
        void removeAfterCurrentIndex(uint32 offset);

        void getCurrentZoneIDs(std::vector<int32> *zoneIDs, uint32 indexCount) const;   //  indexCount: _currentIndex로부터 가져올 zoneID 갯수.
        bool getZoneID(int32 *zoneID, int32 offset) const;  //  offset: _currentIndex로부터 차이.
        std::vector<int32> getAllNextZoneIDs(uint32 offset) const;
        bool isEndZone() const;
        int32 lastJunctionZoneID() const;
        int32 beginZoneID() const;
        int32 endZoneID(uint32 offset = 0) const;   //  offset: endZone으로부터의 차이.

        B1String toString() const;
    };
}   //  !BnD

#endif  // !_B1PATH_INFO_H
