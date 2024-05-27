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
    unsigned int albumTotalTrack;
    std::string genre;
    std::string comment;
    unsigned int bitRate;
    unsigned int sampleRate;
    unsigned int channels;
    unsigned int lengthInSeconds;
    unsigned int lengthInMilliseconds;
    std::vector<Util::Picture> pictures;
    std::string lyrics;
    size_t lyricsLength;
};

ID3v2 *readFromFile(const char *filePath, const ReaderOption &readerOption);
} // namespace FFI::ID3v2
