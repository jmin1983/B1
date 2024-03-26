//
// B1SECS2Consts.h
//
// Library: B1SECS2
// Package: B1SECS2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_CONSTS_H
#define _B1SECS2_CONSTS_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    namespace B1SECS2Consts {
        enum CONTROL_MESSAGE {
            //  stype -> 1:Select.req, 2:Select.rsp, 3:Deselect.req, 4:Deselect.rsp, 5:LinkTest.req, 6:LinkTest.rsp, 7:Reject.req, 9:Seperate.req, 0:data_message
            CONTROL_MESSAGE_SELECT_REQ = 1,
            CONTROL_MESSAGE_SELECT_RSP = 2,
            CONTROL_MESSAGE_DESELECT_REQ = 3,
            CONTROL_MESSAGE_DESELECT_RSP = 4,
            CONTROL_MESSAGE_LINK_TEST_REQ = 5,
            CONTROL_MESSAGE_LINK_TEST_RSP = 6,
            CONTROL_MESSAGE_REJECT_REQ = 7,
            CONTROL_MESSAGE_SEPERATE_REQ = 9,
        };
        enum DATA_FORMAT {
            //  format -> 00:LIST, 10:BINARY, 11:BOOLEAN, 20:ASCII, 21:JIS8, 22:2-BYTE_CHAR(WCHAR),
            //            30:8-BYTE_INT, 31:1-BYTE_INT, 32:2-BYTE_INT, 34:4-BYTE_INT, 40:8-BYTE_FLOAT, 44:4-BYTE_FLOAT, 50:8-BYTE_UINT, 51:1-BYTE_UINT, 52:2-BYTE_UINT, 54:4-BYTE_UINT
            DATA_FORMAT_LIST = 0,
            DATA_FORMAT_BIN = 010,
            DATA_FORMAT_BOOL = 011,
            DATA_FORMAT_ASCII = 020,
            DATA_FORMAT_JIS8 = 021,
            DATA_FORMAT_WCHAR = 022,
            DATA_FORMAT_INT64 = 030,
            DATA_FORMAT_INT8 = 031,
            DATA_FORMAT_INT16 = 032,
            DATA_FORMAT_INT32 = 034,
            DATA_FORMAT_FLOAT64 = 040,
            DATA_FORMAT_FLOAT32 = 044,
            DATA_FORMAT_UINT64 = 050,
            DATA_FORMAT_UINT8 = 051,
            DATA_FORMAT_UINT16 = 052,
            DATA_FORMAT_UINT32 = 054,
        };
    };
}   //  !BnD

#endif  // !_B1SECS2_CONSTS_H
