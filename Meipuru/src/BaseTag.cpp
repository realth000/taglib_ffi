#include "BaseTag.h"
#include <iostream>

namespace Meipuru {

    void BaseTag::print() {
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
                  << "Length: " << length << std::endl;
    }
}
