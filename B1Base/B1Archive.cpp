//
// B1Archive.cpp
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Base.h"
#include "B1Archive.hpp"

#include <boost/property_tree/json_parser.hpp>

using namespace BnD;

B1Archive::B1Archive()
    : _rootNode(new B1ArchiveNode())
{
}

B1Archive::~B1Archive()
{
    delete _rootNode;
}

#define DEFINE_ARCHIVE_WRITE_VALUE_FUNC(type) void B1Archive::writeData(const B1String &key, const type &value) { _rootNode->get().put(key.to_string(), value); }
#define DEFINE_ARCHIVE_READ_VALUE_FUNC(type) bool B1Archive::readData(const B1String &key, type *value) const {\
    try { if (auto child = _rootNode->get().get_child_optional(key.to_string())) { *value = child->get_value<type>(); return true; } }\
    catch (...) {} return false;\
}
#define DEFINE_ARCHIVE_VALUE_FUNC(type) DEFINE_ARCHIVE_WRITE_VALUE_FUNC(type) DEFINE_ARCHIVE_READ_VALUE_FUNC(type)

#define DEFINE_ARCHIVE_WRITE_VALUE_ARRAY_FUNC(type) void B1Archive::writeData(const B1String &key, const type &value) {\
    boost::property_tree::ptree child, temp;\
    for (const auto& v : value) {\
        temp.put_value(v);\
        child.push_back(std::make_pair("", temp));\
    }\
    _rootNode->get().add_child(key.to_string(), child);\
}
#define DEFINE_ARCHIVE_READ_VALUE_ARRAY_FUNC(type) bool B1Archive::readData(const B1String &key, type *value) const {\
    try {\
        value->clear();\
        if (auto child = _rootNode->get().get_child_optional(key.to_string())) {\
            value->reserve(child->size());\
            for (const auto& v : *child) {\
                value->push_back(v.second.get_value<type::value_type>());\
            }\
            return true;\
        }\
    }\
    catch (...) {}\
    return false;\
}
#define DEFINE_ARCHIVE_VALUE_ARRAY_FUNC(type) DEFINE_ARCHIVE_WRITE_VALUE_ARRAY_FUNC(type) DEFINE_ARCHIVE_READ_VALUE_ARRAY_FUNC(type)

DEFINE_ARCHIVE_VALUE_FUNC(bool);
DEFINE_ARCHIVE_VALUE_FUNC(int8);
DEFINE_ARCHIVE_VALUE_FUNC(int16);
DEFINE_ARCHIVE_VALUE_FUNC(int32);
DEFINE_ARCHIVE_VALUE_FUNC(int64);
DEFINE_ARCHIVE_VALUE_FUNC(uint8);
DEFINE_ARCHIVE_VALUE_FUNC(uint16);
DEFINE_ARCHIVE_VALUE_FUNC(uint32);
DEFINE_ARCHIVE_VALUE_FUNC(uint64);
DEFINE_ARCHIVE_VALUE_FUNC(float32);
DEFINE_ARCHIVE_VALUE_FUNC(float64);
DEFINE_ARCHIVE_VALUE_ARRAY_FUNC(std::vector<int8>);
DEFINE_ARCHIVE_VALUE_ARRAY_FUNC(std::vector<int16>);
DEFINE_ARCHIVE_VALUE_ARRAY_FUNC(std::vector<int32>);
DEFINE_ARCHIVE_VALUE_ARRAY_FUNC(std::vector<int64>);
DEFINE_ARCHIVE_VALUE_ARRAY_FUNC(std::vector<uint8>);
DEFINE_ARCHIVE_VALUE_ARRAY_FUNC(std::vector<uint16>);
DEFINE_ARCHIVE_VALUE_ARRAY_FUNC(std::vector<uint32>);
DEFINE_ARCHIVE_VALUE_ARRAY_FUNC(std::vector<uint64>);
DEFINE_ARCHIVE_VALUE_ARRAY_FUNC(std::vector<float32>);
DEFINE_ARCHIVE_VALUE_ARRAY_FUNC(std::vector<float64>);

void B1Archive::writeData(const B1String& key, const B1Object& value)
{
    B1Archive archive;
    value.archiveTo(&archive);
    _rootNode->get().add_child(key.to_string(), archive._rootNode->get());
}

bool B1Archive::readData(const B1String& key, B1Object* value) const
{
    if (auto child = _rootNode->get().get_child_optional(key.to_string())) {
        value->unarchiveFrom(*child);
        return true;
    }
    return false;
}

void B1Archive::writeData(const B1String& key, const B1String& value)
{
    _rootNode->get().put(key.to_string(), value.to_string());
}

bool B1Archive::readData(const B1String& key, B1String* value) const
{
    try {
        if (auto child = _rootNode->get().get_child_optional(key.to_string())) {
            *value = child->get_value<std::string>(); return true;
        }
    }
    catch (...) {}
    return false;
}

void B1Archive::writeData(const B1String& key, const std::vector<B1String>& value)
{
    boost::property_tree::ptree child, temp;
    for (const auto& v : value) {
        temp.put_value(v.to_string());
        child.push_back(std::make_pair("", temp));
    }
    _rootNode->get().add_child(key.to_string(), child);
}

bool B1Archive::readData(const B1String& key, std::vector<B1String>* value) const
{
    try {
        if (auto child = _rootNode->get().get_child_optional(key.to_string())) {
            value->reserve(child->size());
            for (const auto& v : *child) {
                value->push_back(v.second.get_value<std::string>());
            }
            return true;
        }
    }
    catch (...) {}
    return false;
}

void B1Archive::addArchive(const B1Archive& archive)
{
    for (const auto& v : archive._rootNode->get()) {
        _rootNode->get().add_child(v.first, v.second);
    }
}

void B1Archive::pushArchive(const B1Archive& archive)
{
    _rootNode->get().push_back(std::make_pair("", archive._rootNode->get()));
}

bool B1Archive::popArchive(B1Archive* archive) const
{
    if (_rootNode->get().empty())
        return false;
    archive->_rootNode->get() = _rootNode->get().back().second;
    _rootNode->get().pop_back();
    return true;
}

void B1Archive::addSubArchive(const B1String& key, const B1Archive& archive)
{
    try {
        if (auto child = _rootNode->get().get_child_optional(key.to_string())) {
            for (const auto& v : archive._rootNode->get()) {
                child->add_child(v.first, v.second);
            }
        }
        else {
            _rootNode->get().add_child(key.to_string(), archive._rootNode->get());
        }
    }
    catch (...) {}
}

bool B1Archive::getSubArchive(const B1String& key, B1Archive* archive) const
{
    try {
        if (auto child = _rootNode->get().get_child_optional(key.to_string())) {
            archive->_rootNode->get() = *child;
            return true;
        }
    }
    catch (...) {}
    return false;
}

void B1Archive::getFrontArchive(B1Archive* archive) const
{
    archive->_rootNode->get() = _rootNode->get().front().second;
}

void B1Archive::toObject(B1Object* object) const
{
    object->unarchiveFrom(*this);
}

void B1Archive::fromObject(const B1Object& object)
{
    object.archiveTo(this);
}

bool B1Archive::toString(B1String* string, bool pretty) const
{
    std::stringstream ss;
    try {
        boost::property_tree::write_json(ss, _rootNode->get(), pretty);
        string->from(ss.str());
    }
    catch (...) {
        return false;
    }
    return true;
}

bool B1Archive::fromString(const B1String& string)
{
    std::stringstream ss(string.to_string());
    try {
        boost::property_tree::read_json(ss, _rootNode->get());
    }
    catch (...) {
        return false;
    }
    return true;
}
