//
// B1MariaDBResult.h
//
// Library: B1DBClient
// Package: B1DBClient
// Module:  B1DBClient
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1DBCLIENT_MARIA_DB_RESULT_H
#define _B1DBCLIENT_MARIA_DB_RESULT_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <vector>

namespace boost {
    namespace mysql {
        class field_view;
        class rows_view;
    }
}

namespace BnD {
    class B1MariaDBResult {
    public:
        B1MariaDBResult();
        virtual ~B1MariaDBResult();
    protected:
        virtual bool implReadDBResult(const boost::mysql::rows_view& rows) = 0;
    protected:
        int32 toInt32(const boost::mysql::field_view& row) const;
        int64 toInt64(const boost::mysql::field_view& row) const;
        B1String toString(const boost::mysql::field_view& row) const;
        bool getSingleRowResult(const boost::mysql::rows_view& rows, std::vector<B1String>* items, size_t expectedSize = 0) const;
        bool getResult(const boost::mysql::rows_view& rows, std::vector<std::vector<B1String> >* items, size_t expectedSize = 0) const;
        void getResult(const boost::mysql::rows_view& rows, std::vector<B1String>* col0) const;
        bool getResult(const boost::mysql::rows_view& rows, std::vector<std::tuple<int32, B1String, B1String> >* items) const;
        bool getResult(const boost::mysql::rows_view& rows, std::vector<std::tuple<B1String, B1String, std::vector<uint8>, std::vector<uint8>, std::vector<uint8> > >* items) const;
        bool getResult(const boost::mysql::rows_view& rows,
                       std::vector<std::vector<uint8> >* col0,
                       std::vector<std::vector<uint8> >* col1 = NULL,
                       std::vector<std::vector<uint8> >* col2 = NULL) const;
        bool getResult(const boost::mysql::rows_view& rows, std::vector<std::tuple<int64, int32, int32, int32, int32> >* items) const;
    public:
        bool readDBResult(const boost::mysql::rows_view& rows);
    };
}   //  !BnD

#endif  // !_B1DBCLIENT_MARIA_DB_RESULT_H
