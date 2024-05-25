#pragma once

#include "ID3v2.hpp"

namespace FFI {
class Reader {
  public:
    explicit Reader(const ReaderOption &readerOption = ReaderOption());

    ID3v2::ID3v2 *readID3v2TagFromFile(const char *filePath) const;

    ReaderOption option;
};
} // namespace FFI
