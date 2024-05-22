#include "ID3v2Tag.h"

#include <iostream>

namespace Meipuru {
void ID3v2Tag::print() const {
    std::cout << "File: " << fileName << "\n"
              << "File Path: " << filePath << "\n"
              << "Title: " << title << "\n"
              << "Artist: " << artist << "\n"
              << "Album Title: " << albumTitle << "\n"
              << "Album Artist: " << albumArtist << "\n"
              << "Year: " << year << "\n"
              << "Track: " << track << "\n"
              << "Album Track Count: " << albumTotalTrack << "\n"
              << "Genre: " << genre << "\n"
              << "Comment: " << comment << "\n"
              << "Bit Rate: " << bitRate << "\n"
              << "Sample Rate: " << sampleRate << "\n"
              << "Channels: " << channels << "\n"
              << "LengthInSeconds: " << lengthInSeconds << "\n"
              << "lengthInMilliseconds: " << lengthInMilliseconds << "\n"
              << "Album Cover (size): " << this->albumCover->size << "\n"
              << "Lyrics (size): " << lyricsLength << std::endl;
}
} // namespace Meipuru
