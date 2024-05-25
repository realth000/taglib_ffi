#pragma once

#include <cstdint>
#include <string>

#include "toolkit/tbytevector.h"
#include "tstringlist.h"

namespace FFI::Util {
enum class Encoding : int8_t {
    Utf8 = 0,
    Latin1,
};

enum class LogLevel : int8_t {
    None = 0,
    Fatal,
    Critical,
    Error,
    Warning,
    Debug,
};

struct Picture {
    TagLib::ByteVector data;
    std::string mimetype;
    unsigned int size;
};

/// @brief Convert TagLib type `StringList` to a std::vector of std::string.
std::vector<std::string> vecToVec(const TagLib::StringList &stringList);
} // namespace FFI::Util
