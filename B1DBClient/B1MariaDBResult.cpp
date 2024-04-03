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

size_t B1MariaDBResult::rowSize(const boost::mysql::rows_view& rows) const
{
    return rows.size();
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

bool B1MariaDBResult::getResult(const boost::mysql::rows_view& rows, std::vector<std::vector<B1String> >* items) const
{
    items->clear();
    items->reserve(rows.size());
    for (boost::mysql::row_view row : rows) {
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
    for (boost::mysql::row_view row : rows) {
        try {
            auto resultString = toString(row.at(0));
            col0->push_back(std::move(resultString));
        }
        catch (...) { assert(false); }
    }
}

void B1MariaDBResult::getResult(const boost::mysql::rows_view& rows, std::vector<int32>* col0, std::vector<B1String>* col1, std::vector<B1String>* col2) const
{
    for (boost::mysql::row_view row : rows) {
        col0->push_back(static_cast<int32>(row.at(0).as_uint64()));
        col1->push_back(toString(row.at(1)));
        col2->push_back(toString(row.at(2)));
    }
}

void B1MariaDBResult::getResult(const boost::mysql::rows_view& rows,
                                std::vector<B1String>* col0, std::vector<B1String>* col1,
                                std::vector<std::vector<uint8> >* col2, std::vector<std::vector<uint8> >* col3, std::vector<std::vector<uint8> >* col4) const
{
    for (boost::mysql::row_view row : rows) {
        {
            col0->push_back(toString(row.at(0)));
            col1->push_back(toString(row.at(1)));
            {
                auto blob = row.at(2).as_blob();
                col2->push_back(std::vector<uint8>(blob.begin(), blob.end()));
            }
            {
                auto blob = row.at(3).as_blob();
                col3->push_back(std::vector<uint8>(blob.begin(), blob.end()));
            }
            {
                auto blob = row.at(4).as_blob();
                col4->push_back(std::vector<uint8>(blob.begin(), blob.end()));
            }
        }
    }
}

void B1MariaDBResult::getResult(const boost::mysql::rows_view& rows,
                                std::vector<std::vector<uint8> >* col0,
                                std::vector<std::vector<uint8> >* col1,
                                std::vector<std::vector<uint8> >* col2) const
{
    for (boost::mysql::row_view row : rows) {
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
}

bool B1MariaDBResult::readDBResult(const boost::mysql::rows_view& rows)
{
    return implReadDBResult(rows);
}
