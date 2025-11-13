//
// B1HSMSMessage.h
//
// Library: B1SECS2
// Package: B1SECS2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_HSMS_MESSAGE_H
#define _B1SECS2_HSMS_MESSAGE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    struct B1HSMSMessage {
        enum TIME_OUT {
            TIME_OUT_T3 = 45 * 1000,    //  T3(Reply Timeout) range:1~120sec, default:45sec
            TIME_OUT_T6 = 5 * 1000,     //  T5(Connect Separation Timeout) range:1~240sec, default:5sec
            TIME_OUT_T7 = 10 * 1000,    //  T7(NOT SELECTED Timeout) range:1~240sec, default:10sec
            TIME_OUT_T8 = 5 * 1000,     //  T8(Network Intercharacter Timeout) range:1~120sec, default:5sec
        };
        B1HSMSMessage() : _recved(0), _length(0), _data() {}
        uint32 _recved;
        uint32 _length;
        std::vector<uint8> _data;   //  10 byte header included
        void reset()
        {
            _recved = 0;
            _length = 0;
            _data.clear();
        }
    };
}   //  !BnD

#endif  // !_B1SECS2_HSMS_MESSAGE_H
