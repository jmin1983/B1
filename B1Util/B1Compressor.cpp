//
// B1Compressor.cpp
//
// Library: B1Util
// Package: B1Util
// Module:  B1Util
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Util.h"
#include "B1Compressor.h"

#include <zlib/zlib.h>

using namespace BnD;

auto B1Compressor::compress(const std::vector<uint8>& data, const size_t bufferSize) -> std::vector<uint8>
{
    if (data.empty()) {
        assert(false);
        return std::vector<uint8>();
    }
    z_stream zStream;
    zStream.zalloc = Z_NULL;
    zStream.zfree = Z_NULL;
    zStream.opaque = Z_NULL;
    auto returnValue = deflateInit(&zStream, Z_DEFAULT_COMPRESSION);
    if (returnValue != Z_OK) {
        return std::vector<uint8>();
    }

    size_t dataPos = 0;
    std::vector<uint8> packed;
    packed.reserve(data.size());
    do {
        if (dataPos >= data.size()) {
            assert(false);
            return std::vector<uint8>();
        }
        int32 reamins = data.size() - dataPos;
        size_t currentDataSize = bufferSize < reamins ? bufferSize : reamins;
        zStream.avail_in = currentDataSize;
        zStream.next_in = (Bytef*)&data[dataPos];
        auto flush = dataPos + currentDataSize == data.size() ? Z_FINISH : Z_NO_FLUSH;
        do {    //  run deflate() on input until output buffer not full, finish compression if all of source has been read in.
            zStream.avail_out = bufferSize;
            std::vector<uint8> buffer(bufferSize, 0);
            zStream.next_out = &buffer[0];
            returnValue = deflate(&zStream, flush); // no bad return value.
            assert(returnValue != Z_STREAM_ERROR);  // state not clobbered.
            size_t writtenSize = bufferSize - zStream.avail_out;
            if (buffer.size() < writtenSize) {
                assert(false);
                return std::vector<uint8>();
            }
            packed.insert(packed.end(), buffer.begin(), buffer.begin() + writtenSize);
        } while (zStream.avail_out == 0);
        assert(zStream.avail_in == 0);  //  all input will be used.
        dataPos += currentDataSize;
    } while (dataPos != data.size());
    assert(returnValue == Z_STREAM_END);    //  stream will be complete.
    (void)deflateEnd(&zStream); //  clean up and return.
    return packed;
}

auto B1Compressor::decompress(const std::vector<uint8>& data, const size_t bufferSize) -> std::vector<uint8>
{
    if (data.empty()) {
        assert(false);
        return std::vector<uint8>();
    }
    z_stream zStream;
    zStream.zalloc = Z_NULL;
    zStream.zfree = Z_NULL;
    zStream.opaque = Z_NULL;
    zStream.avail_in = 0;
    zStream.next_in = Z_NULL;
    auto returnValue = inflateInit(&zStream);
    if (returnValue != Z_OK) {
        return std::vector<uint8>();
    }

    size_t dataPos = 0;
    std::vector<uint8> unpacked;
    do {
        if (dataPos >= data.size()) {
            assert(false);
            return std::vector<uint8>();
        }
        int32 reamins = data.size() - dataPos;
        size_t currentDataSize = bufferSize < reamins ? bufferSize : reamins;
        zStream.avail_in = currentDataSize;
        zStream.next_in = (Bytef*)&data[dataPos];
        do {    //  run inflate() on input until output buffer not full.
            zStream.avail_out = bufferSize;
            std::vector<uint8> buffer(bufferSize, 0);
            zStream.next_out = &buffer[0];
            returnValue = inflate(&zStream, Z_NO_FLUSH);
            assert(returnValue != Z_STREAM_ERROR);  //  state not clobbered.
            switch (returnValue) {
                case Z_NEED_DICT:
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    (void)inflateEnd(&zStream);
                    return std::vector<uint8>();
                default:
                    break;
            }
            size_t writtenSize = bufferSize - zStream.avail_out;
            if (buffer.size() < writtenSize) {
                assert(false);
                return std::vector<uint8>();
            }
            unpacked.insert(unpacked.end(), buffer.begin(), buffer.begin() + writtenSize);
        } while (zStream.avail_out == 0);
        dataPos += currentDataSize;
    } while (returnValue != Z_STREAM_END);
    (void)inflateEnd(&zStream);
    return unpacked;
}