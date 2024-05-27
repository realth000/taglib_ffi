#include "Reader.hpp"

namespace FFI {

Reader::Reader(const ReaderOption &readerOption) : option(readerOption) {}

ID3v2::ID3v2 *Reader::readID3v2TagFromFile(const char *filePath) const { return ID3v2::readFromFile(filePath, option); }

} // namespace FFI
