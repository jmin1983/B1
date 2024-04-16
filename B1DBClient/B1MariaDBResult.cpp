//
// B1MariaDBResult.cpp
//
// Library: B1DBClient
// Package: B1DBClient
// Module:  B1DBClient
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1DBClient.h"
#include "B1MariaDBResult.h"

using namespace BnD;

B1MariaDBResult::B1MariaDBResult()
{
}

B1MariaDBResult::~B1MariaDBResult()
{
}

B1String B1MariaDBResult::toString(const boost::mysql::field_view& row) const
{
    if (row.is_string()) {
        auto str = row.as_string();
        std::vector<char> temp(str.length() + 1, 0);
        memcpy(&temp[0], str.begin(), str.length());
        return &temp[0];
    }
    return "";
}

bool B1MariaDBResult::getSingleRowResult(const boost::mysql::rows_view& rows, std::vector<B1String>* items, size_t expectedSize) const
{
    if (rows.empty()) {
        items->clear();
        if (expectedSize > 0) {
            return false;
        }
    }
    else {
        size_t size = rows.front().size();
        if (expectedSize > 0 && expectedSize != size) {
            return false;
        }
        items->resize(size);
        for (size_t i = 0; i < size; ++i) {
            try {
                items->push_back(toString(rows.front().at(i)));
            }
            catch (...) {
                return false;
            }
        }
    }
    return true;
}

bool B1MariaDBResult::getResult(const boost::mysql::rows_view& rows, std::vector<std::vector<B1String> >* items, size_t expectedSize) const
{
    items->clear();
    if (expectedSize > 0 && expectedSize != rows.size()) {
        return false;
    }
    items->reserve(rows.size());
    for (auto row : rows) {
        try {
            std::vector<B1String> item;
            item.reserve(row.size());
            for (const auto& r : row) {
                item.push_back(toString(r));
            }
            items->push_back(std::move(item));
        }
        catch (...) {
            assert(false);
            return false;
        }
    }
    return true;
}

void B1MariaDBResult::getResult(const boost::mysql::rows_view& rows, std::vector<B1String>* col0) const
{
    col0->clear();
    col0->reserve(rows.size());
    for (auto row : rows) {
        try {
            auto resultString = toString(row.at(0));
            col0->push_back(std::move(resultString));
        }
        catch (...) { assert(false); }
    }
}

bool B1MariaDBResult::getResult(const boost::mysql::rows_view& rows, std::vector<std::tuple<int32, B1String, B1String> >* items) const
{
    items->clear();
    items->reserve(rows.size());
    for (auto row : rows) {
        try {
            items->push_back(std::make_tuple<int32, B1String, B1String>(static_cast<int32>(row.at(0).as_int64()), toString(row.at(1)), toString(row.at(2))));
        }
        catch (...) {
            return false;
        }
    }
    return true;
}

bool B1MariaDBResult::getResult(const boost::mysql::rows_view& rows, std::vector<std::tuple<B1String, B1String, std::vector<uint8>, std::vector<uint8>, std::vector<uint8> > >* items) const
{
    items->clear();
    items->reserve(rows.size());
    for (auto row : rows) {
        try {
            auto blob2 = row.at(2).as_blob();
            auto blob3 = row.at(3).as_blob();
            auto blob4 = row.at(4).as_blob();
            items->push_back(std::make_tuple<B1String, B1String, std::vector<uint8>, std::vector<uint8>, std::vector<uint8> >(
                toString(row.at(0)),
                toString(row.at(1)),
                std::vector<uint8>(blob2.begin(), blob2.end()),
                std::vector<uint8>(blob3.begin(), blob3.end()),
                std::vector<uint8>(blob4.begin(), blob4.end())));
        }
        catch (...) {
            return false;
        }
    }
    return true;
}

bool B1MariaDBResult::getResult(const boost::mysql::rows_view& rows,
                                std::vector<std::vector<uint8> >* col0,
                                std::vector<std::vector<uint8> >* col1,
                                std::vector<std::vector<uint8> >* col2) const
{
    col0->clear();
    col1->clear();
    col2->clear();
    col0->reserve(rows.size());
    col1->reserve(rows.size());
    col2->reserve(rows.size());
    for (auto row : rows) {
        try {
            {
                auto blob = row.at(0).as_blob();
                col0->push_back(std::vector<uint8>(blob.begin(), blob.end()));
            }
            if (col1) {
                auto blob = row.at(1).as_blob();
                col1->push_back(std::vector<uint8>(blob.begin(), blob.end()));
            }
            if (col2) {
                auto blob = row.at(2).as_blob();
                col2->push_back(std::vector<uint8>(blob.begin(), blob.end()));
            }
        }
        catch (...) {
            return false;
        }
    }
    return true;
}

bool B1MariaDBResult::getResult(const boost::mysql::rows_view& rows, std::vector<std::tuple<int64, int32, int32, int32, int32> >* items) const
{
    items->clear();
    items->reserve(rows.size());
    for (auto row : rows) {
        try {
            items->push_back(std::make_tuple<int64, int32, int32, int32, int32>(
                row.at(0).as_int64(),
                static_cast<int32>(row.at(1).as_int64()),
                static_cast<int32>(row.at(2).as_int64()),
                static_cast<int32>(row.at(3).as_int64()),
                static_cast<int32>(row.at(4).as_int64())));
        }
        catch (...) {
            return false;
        }
    }
    return true;
}

bool B1MariaDBResult::readDBResult(const boost::mysql::rows_view& rows)
{
    return implReadDBResult(rows);
}
