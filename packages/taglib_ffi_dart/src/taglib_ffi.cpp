#include "taglib_ffi.h"

#include "src/Reader.hpp"

struct Id3v2TagOwner {
    // On heap.
    const FFI::ID3v2::ID3v2 *tag;
};

const ID3v2Tag *readID3v2Tag(const char *filePath, bool readImage) {
    const auto readerOption = FFI::ReaderOption{
        FFI::Util::Encoding::Utf8,
        FFI::Util::LogLevel::None,
        true,
    };
    const auto reader = FFI::Reader(readerOption);
    const auto id3v2Tag = reader.readID3v2TagFromFile(filePath);

    if (id3v2Tag == nullptr) {
        return nullptr;
    }

    const auto tag = new ID3v2Tag{
        id3v2Tag->filePath.c_str(),
        id3v2Tag->fileName.c_str(),
        id3v2Tag->title.c_str(),
        nullptr, // Aritst
        id3v2Tag->artist.size(),
        id3v2Tag->albumTitle.c_str(),
        nullptr, // Album artist
        id3v2Tag->albumArtist.size(),
        id3v2Tag->year,
        id3v2Tag->track,
        id3v2Tag->albumTotalTrack,
        id3v2Tag->genre.c_str(),
        id3v2Tag->comment.c_str(),
        id3v2Tag->bitRate,
        id3v2Tag->sampleRate,
        id3v2Tag->channels,
        id3v2Tag->lengthInSeconds,
        id3v2Tag->lengthInMilliseconds,
        nullptr, // Picture
        id3v2Tag->pictures.size(),
        id3v2Tag->lyrics.c_str(),
        new Id3v2TagOwner{id3v2Tag},
    };
    // FIXME:
    tag->artist = (const char **)malloc(sizeof(char *) * id3v2Tag->artist.size());
    for (auto i = 0; i < id3v2Tag->artist.size(); i++) {
        tag->artist[i] = id3v2Tag->artist[i].c_str();
    }

    return tag;
}

void freeID3v2Tag(const ID3v2Tag *tag) {
    free(tag->artist);
    delete tag->_owner->tag;
    delete tag->_owner;
    delete tag;
}

// FFI_PLUGIN_EXPORT void printID3v2Tag(ID3v2Tag *id3V2Tag) {
//     if (id3V2Tag == nullptr) {
//         std::cout << "ptr is empty" << std::endl;
//         return;
//     }
//
//     std::cout << "File: " << id3V2Tag->fileName << "\n"
//               << "File Path: " << id3V2Tag->filePath << "\n"
//               << "Title: " << id3V2Tag->title << "\n"
//               << "Artist: " << id3V2Tag->artist << "\n"
//               << "Album Title: " << id3V2Tag->albumTitle << "\n"
//               << "Album Artist: " << id3V2Tag->albumArtist << "\n"
//               << "Year: " << id3V2Tag->year << "\n"
//               << "Track: " << id3V2Tag->track << "\n"
//               << "Album Track Count: " << id3V2Tag->albumTotalTrack << "\n"
//               << "Genre: " << id3V2Tag->genre << "\n"
//               << "Comment: " << id3V2Tag->comment << "\n"
//               << "Bit Rate: " << id3V2Tag->bitRate << "\n"
//               << "Sample Rate: " << id3V2Tag->sampleRate << "\n"
//               << "Channels: " << id3V2Tag->channels << "\n"
//               << "LengthInSeconds: " << id3V2Tag->lengthInSeconds << "\n"
//               << "LengthInMilliseconds: " << id3V2Tag->lengthInMilliSeconds << "\n"
//               << "Album Cover (size): " << id3V2Tag->albumCoverLength << "\n"
//               << "Lyrics (size): " << id3V2Tag->lyricsLength << std::endl;
// }
