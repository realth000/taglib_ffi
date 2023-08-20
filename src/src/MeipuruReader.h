#ifndef MEIPURU_MEIPURUREADER_H
#define MEIPURU_MEIPURUREADER_H

#include <iostream>

#include "fileref.h"
#include "tag.h"

#include "BaseTag.h"
#include "ID3v2Tag.h"
#include "mpegfile.h"

namespace Meipuru {
class MeipuruReaderOption {
public:
  explicit MeipuruReaderOption();

  bool useUnicode() const;

  Util::Encoding encoding;
  Util::LogLevel logLevel;

private:
  bool unicode;
};

class MeipuruReader {
public:
  explicit MeipuruReader(
      const MeipuruReaderOption &meipuruReaderOption = MeipuruReaderOption());

  BaseTag *readTagFromFile(const char *filePath);
  ID3v2Tag *readID3v2TagFromFile(const char *filePath);

private:
  bool fetchBaseTag(const TagLib::File *file, BaseTag *baseTag) const;

  MeipuruReaderOption option;
};
} // namespace Meipuru

#endif // MEIPURU_MEIPURUREADER_H
