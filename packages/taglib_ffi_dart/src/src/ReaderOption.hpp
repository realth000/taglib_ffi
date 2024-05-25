#pragma once

#include "Utils.hpp"

namespace FFI {
struct ReaderOption {
    Util::Encoding encoding = Util::Encoding::Utf8;
    Util::LogLevel logLevel = Util::LogLevel::None;
    bool readImage = true;
};

} // namespace FFI
