#pragma once

#include "ReaderOption.hpp"
#include "Utils.hpp"

namespace FFI::ID3v2 {

class ID3v2 {
  public:
    void print() const;

    std::string filePath;
    std::string fileName;
    std::string title;
    std::vector<std::string> artist;
    std::string albumTitle;
    std::vector<std::string> albumArtist;
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
    std::vector<Util::Picture> albumCover;
    std::string lyrics;
    size_t lyricsLength;
};

ID3v2 *readFromFile(const char *filePath, const ReaderOption &readerOption);
} // namespace FFI::ID3v2
