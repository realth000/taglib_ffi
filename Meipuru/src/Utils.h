#include <iostream>

namespace Meipuru::Util {
enum Encoding : std::int8_t {
  Unicode = 0,
  Latin1,
};

enum LogLevel : std::int8_t { Fatal = 0, Critical, Error, Warning, Debug };

struct Picture {
  char *data;
  std::string mimetype;
  unsigned int size;
};
}  // namespace Meipuru::Util