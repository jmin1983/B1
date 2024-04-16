//
// B1Archive.hpp
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1LIB_B1BASE_ARCHIVE_HPP
#define _B1LIB_B1BASE_ARCHIVE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "B1Archive.h"
#include "B1ArchiveNode.h"
#include "B1Object.h"
#include "B1String.h"

#include <boost/property_tree/ptree.hpp>

namespace BnD {
    template <typename T>
    B1Archive::B1Archive(const T& t)
        : _rootNode(new B1ArchiveNode())
    {
        _rootNode->_ptree = t;
    }

    template<typename T>
    void B1Archive::writeData(const B1String& key, const std::unordered_map<B1String, T>& value)
    {
        boost::property_tree::ptree child, temp;
        for (const auto& v : value) {
            child.put(v.first.to_string(), v.second);
        }
        _rootNode->get().add_child(key.to_string(), child);
    }

    template<typename T>
    bool B1Archive::readData(const B1String& key, std::unordered_map<B1String, T>* value) const
    {
        try {
            if (auto child = _rootNode->get().get_child_optional(key.to_string())) {
                for (const auto& v : *child) {
                    value->insert(std::make_pair(v.first, v.second.get_value<T>()));
                }
                return true;
            }
        }
        catch (...) {}
        return false;
    }

    template<typename T>
    void B1Archive::writeObjectArray(const B1String& key, const std::vector<T>& value)
    {
        boost::property_tree::ptree child;
        for (const B1Object& v : value) {
            B1Archive archive;
            v.archiveTo(&archive);
            child.push_back(std::make_pair("", archive._rootNode->get()));
        }
        _rootNode->get().add_child(key.to_string(), child);
    }

    template<typename T>
    void B1Archive::readObjectArray(const B1String& key, std::vector<T>* value) const
    {
        value->clear();
        try {
            if (auto child = _rootNode->get().get_child_optional(key.to_string())) {
                value->reserve(child->size());
                for (const auto& v : *child) {
                    T t;
                    dynamic_cast<B1Object&>(t).unarchiveFrom(v.second);
                    value->push_back(std::move(t));
                }
            }
        }
        catch (...) {}
    }
}   //  !J2Sol

#endif  // !_B1LIB_B1BASE_ARCHIVE_HPP
