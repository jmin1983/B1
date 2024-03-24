//
// B1ArchiveNode.h
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1BASE_ARCHIVE_NODE_H
#define _B1BASE_ARCHIVE_NODE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/property_tree/ptree.hpp>

namespace BnD {
    class B1ArchiveNode {
    public:
        boost::property_tree::ptree _ptree;
        boost::property_tree::ptree& get() { return _ptree; }
    };
}   //  !BnD

#endif  // !_B1BASE_ARCHIVE_NODE_H
