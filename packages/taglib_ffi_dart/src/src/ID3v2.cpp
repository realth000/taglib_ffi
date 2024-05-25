#include "ID3v2.hpp"
#include <attachedpictureframe.h>

#include <id3v2tag.h>
#include <iostream>
#include <mpegfile.h>
#include <tpropertymap.h>

namespace FFI::ID3v2 {

// All used keys in FrameListMap.
namespace Frame {
static constexpr auto picture = "APIC";
static constexpr auto unsyncLyrics = "USLT";
} // namespace Frame

// All used keys in PropertyMap.
namespace Property {
static constexpr auto artist = "ARTIST";
static constexpr auto albumArtist = "ALBUMARTIST";
static constexpr auto trackNumber = "TRACKNUMBER";
} // namespace Property

void ID3v2::print() const {
    std::cout << "File: " << fileName << "\n"
              << "File Path: " << filePath << "\n"
              << "Title: " << title
              << "\n"
              // << "Artist: " << artist << "\n"
              << "Album Title: " << albumTitle
              << "\n"
              // << "Album Artist: " << albumArtist << "\n"
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
              << "Album Cover (count): " << this->albumCover.size() << "\n"
              << "Lyrics (size): " << lyricsLength << std::endl;
}

ID3v2 *readFromFile(const char *filePath, const ReaderOption &readerOption) {
    auto mpegFile = TagLib::MPEG::File{filePath};
    if (!mpegFile.isValid() || !mpegFile.hasID3v2Tag()) {
        return nullptr;
    }

    const auto id3v2Tag = mpegFile.ID3v2Tag();
    if (id3v2Tag == nullptr) {
        return nullptr;
    }

    const auto tag = mpegFile.tag();
    if (tag == nullptr) {
        return nullptr;
    }
    auto retTag = new ID3v2();
    retTag->filePath = filePath;
    retTag->fileName = mpegFile.name();
    retTag->title = tag->title().to8Bit(true);
    retTag->albumTitle = tag->album().to8Bit(true);
    retTag->year = tag->year();
    retTag->track = tag->track();
    retTag->genre = tag->genre().to8Bit(true);
    retTag->comment = tag->comment().to8Bit(true);

    const auto properties = mpegFile.properties();
    retTag->artist = Util::vecToVec(properties[Property::artist]);
    retTag->albumArtist = Util::vecToVec(properties[Property::albumArtist]);
    const auto trackNumberStr = properties[Property::trackNumber].toString();
    if (!trackNumberStr.isEmpty()) {
        const auto pos = trackNumberStr.split("/");
        if (pos.size() == 2) {
            // "1/20"
            retTag->track = pos[0].toInt();
            retTag->albumTotalTrack = pos[1].toInt();
        } else if (trackNumberStr[0] == '/') {
            // "/20"
            retTag->track = 0;
            retTag->albumTotalTrack = pos[1].toInt();
        } else {
            // "1"
            retTag->track = trackNumberStr.toInt();
        }
    } else {
        // Empty track info.
        retTag->track = 0;
        retTag->albumTotalTrack = 0;
    }

    // Read audio properties.
    //
    // These properties are supposed to be NOT empty.
    const auto audioProperties = mpegFile.audioProperties();
    if (audioProperties == nullptr) {
        // Usually not this branch.
        retTag->bitRate = 0;
        retTag->sampleRate = 0;
        retTag->channels = 0;
        retTag->lengthInSeconds = 0;
        retTag->lengthInMilliseconds = 0;
    } else {
        retTag->bitRate = audioProperties->bitrate();
        retTag->sampleRate = audioProperties->sampleRate();
        retTag->channels = audioProperties->channels();
        retTag->lengthInSeconds = audioProperties->lengthInSeconds();
        retTag->lengthInMilliseconds = audioProperties->lengthInMilliseconds();
    }

    const auto frames = id3v2Tag->frameListMap();

    // Read lyrics.
    if (frames.contains(Frame::unsyncLyrics) && !frames[Frame::unsyncLyrics].isEmpty()) {
        const auto unsyncLyricsStr = frames[Frame::unsyncLyrics].front()->toString();
        retTag->lyrics = unsyncLyricsStr.to8Bit(true);
        retTag->lyricsLength = unsyncLyricsStr.length();
    } else {
        retTag->lyrics = "";
        retTag->lyricsLength = 0;
    }

    // Read cover images.
    if (readerOption.readImage && frames.contains(Frame::picture)) {
        const auto allPictures = frames[Frame::picture];
        const auto count = allPictures.size();
        if (count <= 0) {
            retTag->albumCover = {};
        } else {
            retTag->albumCover = std::vector<Util::Picture>{count};
            for (auto i = 0; i < count; i++) {
                const auto image = dynamic_cast<const TagLib::ID3v2::AttachedPictureFrame *>(allPictures[i]);
                retTag->albumCover[i] = {
                    image->picture(),
                    image->mimeType().to8Bit(true),
                    image->picture().size(),
                };
            };
        }
    } else {
        retTag->albumCover = {};
    }

    return retTag;
}
} // namespace FFI::ID3v2
