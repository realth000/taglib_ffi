#pragma once

#include "Utils.h"

namespace Meipuru {
class ID3v2Tag {
  public:
    void print() const;

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
    std::unique_ptr<Util::Picture> albumCover;
    std::string lyrics;
    size_t lyricsLength;
};
} // namespace Meipuru
