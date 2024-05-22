#pragma once

#include <cstdint>
#include <string>

#include "toolkit/tbytevector.h"

namespace Meipuru::Util {
enum Encoding : int8_t {
    Unicode = 0,
    Latin1,
};

enum class LogLevel : int8_t { Fatal = 0, Critical, Error, Warning, Debug };

struct Picture {
    TagLib::ByteVector data;
    std::string mimetype;
    unsigned int size;
};
} // namespace Meipuru::Util
