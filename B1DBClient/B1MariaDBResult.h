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
        B1String toString(const boost::mysql::field_view& row) const;
        size_t rowSize(const boost::mysql::rows_view& rows) const;
        bool getSingleRowResult(const boost::mysql::rows_view& rows, std::vector<B1String>* items, size_t expectedSize = 0) const;
        bool getResult(const boost::mysql::rows_view& rows, std::vector<std::vector<B1String> >* items) const;
        void getResult(const boost::mysql::rows_view& rows, std::vector<B1String>* col0) const;
        void getResult(const boost::mysql::rows_view& rows, std::vector<int32>* col0, std::vector<B1String>* col1, std::vector<B1String>* col2) const;
        void getResult(const boost::mysql::rows_view& rows,
                       std::vector<B1String>* col0, std::vector<B1String>* col1,
                       std::vector<std::vector<uint8> >* col2, std::vector<std::vector<uint8> >* col3, std::vector<std::vector<uint8> >* col4) const;
        void getResult(const boost::mysql::rows_view& rows,
                       std::vector<std::vector<uint8> >* col0,
                       std::vector<std::vector<uint8> >* col1 = NULL,
                       std::vector<std::vector<uint8> >* col2 = NULL) const;
    public:
        bool readDBResult(const boost::mysql::rows_view& rows);
    };
}   //  !BnD

#endif  // !_B1DBCLIENT_MARIA_DB_RESULT_H
