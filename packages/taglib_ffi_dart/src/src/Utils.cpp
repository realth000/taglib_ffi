#include "Utils.hpp"
namespace FFI::Util {

std::vector<std::string> vecToVec(const TagLib::StringList &stringList) {
    const auto length = stringList.size();
    if (length == 0) {
        return {};
    }
    auto vec = std::vector<std::string>{length};

    for (auto i = 0; i < length; i++) {
        vec[i] = stringList[i].to8Bit(true);
    }

    return vec;
}
} // namespace FFI::Util
