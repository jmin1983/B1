//
// B1BasePacketAnalyzer.h
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1NETWORK_BASE_PACKET_ANALYZER_H
#define _B1NETWORK_BASE_PACKET_ANALYZER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    class B1BasePacketAnalyzer {
    public:
        B1BasePacketAnalyzer(size_t defaultBufferSize = CONSTS_DEFAULT_BUFFER_RESERVED_SIZE);
        virtual ~B1BasePacketAnalyzer();
    protected:
        enum CONSTS {
            CONSTS_MAX_DATA_SIZE = 1024 * 1024 * 100,
            CONSTS_DEFAULT_BUFFER_RESERVED_SIZE = 1024 * 4,
        };
        enum ANALYZE_RESULT {
            ANALYZE_RESULT_SUCCESS = 0,
            ANALYZE_RESULT_FAIL = 1,
            ANALYZE_RESULT_NOT_ENOUTH_DATA = 2,
        };
        std::vector<uint8> _recvdBuffer;
    protected:
        virtual ANALYZE_RESULT implAnalyzeData(uint8* data, size_t size, size_t* pos) = 0;
    protected:
        bool analyzeData(const uint8* data, size_t size);
        void clearBuffer(); //  WARNING: thread un-safe
    };
}   //  !BnD

#endif  // !_B1NETWORK_BASE_PACKET_ANALYZER_H
