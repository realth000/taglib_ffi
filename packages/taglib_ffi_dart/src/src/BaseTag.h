#ifndef MEIPURU_BASETAG_H
#define MEIPURU_BASETAG_H

#include <string>

namespace Meipuru {
class BaseTag {
public:
  virtual ~BaseTag() = default;
  virtual void print();

  std::string filePath;
  std::string fileName;
  std::string title;
  std::string artist;
  std::string albumTitle;
  std::string albumArtist;
  unsigned int year;
  unsigned int track;
  int albumTotalTrack;
  std::string genre;
  std::string comment;
  int bitRate;
  int sampleRate;
  int channels;
  int lengthInSeconds;
  int lengthInMilliseconds;
};
} // namespace Meipuru

#endif // MEIPURU_BASETAG_H
