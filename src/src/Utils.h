#include "toolkit/tbytevector.h"
#include <cstdint>
#include <iostream>
#include <vector>

namespace Meipuru::Util {
enum Encoding : int8_t {
  Unicode = 0,
  Latin1,
};

enum LogLevel : int8_t { Fatal = 0, Critical, Error, Warning, Debug };

struct Picture {
  TagLib::ByteVector data;
  std::string mimetype;
  unsigned int size;
};
} // namespace Meipuru::Util