#pragma once

#include "ID3v2Tag.h"

namespace Meipuru {
class MeipuruReaderOption {
  public:
    explicit MeipuruReaderOption();

    [[nodiscard]] bool useUnicode() const;

    Util::Encoding encoding;
    Util::LogLevel logLevel;

  private:
    bool unicode;
};

class MeipuruReader {
  public:
    explicit MeipuruReader(const MeipuruReaderOption &meipuruReaderOption = MeipuruReaderOption());

    ID3v2Tag *readID3v2TagFromFile(const char *filePath, bool readImage) const;

    MeipuruReaderOption option;
};
} // namespace Meipuru
