//
// B1PathFinderListener.h
//
// Library: B1Path
// Package: B1Path
// Module:  B1Path
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1PATH_FINDER_LISTENER_H
#define _B1PATH_FINDER_LISTENER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    class B1PathInfo;
    class B1PathFinderListener {
    public:
        virtual void onPathFinderResult(int32 taskID, std::shared_ptr<B1PathInfo> result, const void *param) {}
        virtual void onPathFinderFail(int32 taskID, int32 zoneIDFrom, int32 zoneIDTo, const std::vector<int32> &junctions, const void *param) {}
    };
}   //  !BnD

#endif  // !_B1PATH_FINDER_LISTENER_H
