#include "Reader.hpp"

#include <attachedpictureframe.h>
#include <mpegfile.h>
#include <tpropertymap.h>

#include "id3v2tag.h"

namespace FFI {

Reader::Reader(const ReaderOption &readerOption) : option(readerOption) {}

ID3v2::ID3v2 *Reader::readID3v2TagFromFile(const char *filePath) const {
    ID3v2::readFromFile(filePath, readerOption);
}

} // namespace FFI
