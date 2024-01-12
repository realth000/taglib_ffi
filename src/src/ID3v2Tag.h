#ifndef MEIPURU_ID3V2TAG_H
#define MEIPURU_ID3V2TAG_H

#include "BaseTag.h"
#include "Utils.h"

namespace Meipuru {
class ID3v2Tag : public BaseTag {
 public:
  void print() override;

  Util::Picture albumCover;
  std::string lyrics;
  size_t lyricsLength;
};
}  // namespace Meipuru

#endif  // MEIPURU_ID3V2TAG_H
