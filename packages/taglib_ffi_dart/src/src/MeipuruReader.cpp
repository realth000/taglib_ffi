#include "MeipuruReader.h"

#include "id3v2tag.h"
#include "mpegfile.h"
#include "taglib/taglib/mpeg/id3v2/frames/attachedpictureframe.h"
#include "tpropertymap.h"

namespace Meipuru {
MeipuruReaderOption::MeipuruReaderOption() {
    encoding = Util::Encoding::Unicode;
    logLevel = Util::LogLevel::Error;
    unicode = true;
}

bool MeipuruReaderOption::useUnicode() const { return unicode; }

MeipuruReader::MeipuruReader(const MeipuruReaderOption &meipuruReaderOption) : option(meipuruReaderOption) {}

ID3v2Tag *MeipuruReader::readID3v2TagFromFile(const char *filePath, bool readImage) const {
    TagLib::MPEG::File mpegFile(filePath);
    if (!mpegFile.isValid()) {
        return nullptr;
    }
    if (!mpegFile.hasID3v2Tag()) {
        return nullptr;
    }
    auto id3v2Tag = mpegFile.ID3v2Tag();
    if (id3v2Tag == nullptr) {
        return nullptr;
    }
    const bool useUnicode = option.useUnicode();
    auto retTag = new ID3v2Tag;
    retTag->filePath = filePath;
    retTag->fileName = mpegFile.name();
    const auto *tag = mpegFile.tag();
    if (tag == nullptr) {
        return nullptr;
    }
    retTag->title = tag->title().to8Bit(useUnicode);
    retTag->artist = tag->artist().to8Bit(useUnicode);
    retTag->albumTitle = tag->album().to8Bit(useUnicode);
    retTag->year = tag->year();
    retTag->track = tag->track();
    retTag->genre = tag->genre().to8Bit(useUnicode);
    retTag->comment = tag->comment().to8Bit(useUnicode);

    const auto propertyMap = mpegFile.properties();
    // TODO: albumArtist is TagLib::string, should be std::string types.
    retTag->albumArtist = propertyMap["ALBUMARTIST"].toString().to8Bit(useUnicode);
    const auto trackNumberString = propertyMap["TRACKNUMBER"].toString();
    if (!trackNumberString.isEmpty()) {
        const auto pos = trackNumberString.split("/");
        if (pos.size() == 2) {
            // "1/20"
            retTag->track = pos[0].toInt();
            retTag->albumTotalTrack = pos[1].toInt();
        } else if (trackNumberString[0] == '/') {
            // "/20"
            retTag->albumTotalTrack = pos[1].toInt();
        } else {
            // "1"
            retTag->track = trackNumberString.toInt();
        }
    }
    const auto *audioProperties = mpegFile.audioProperties();
    if (audioProperties != nullptr) {
        retTag->bitRate = audioProperties->bitrate();
        retTag->sampleRate = audioProperties->sampleRate();
        retTag->channels = audioProperties->channels();
        retTag->lengthInSeconds = audioProperties->lengthInSeconds();
        retTag->lengthInMilliseconds = audioProperties->lengthInMilliseconds();
    } else {
        retTag->bitRate = 0;
        retTag->sampleRate = 0;
        retTag->channels = 0;
        retTag->lengthInSeconds = 0;
        retTag->lengthInMilliseconds = 0;
    }

    const auto &frameListMap = id3v2Tag->frameListMap();
    // TODO: Handle synchronous lyrics.
    // if (!frameListMap["SYLT"].isEmpty() && frameListMap["SYLT"].front() !=
    // nullptr) {
    //     retTag->lyrics =
    //     frameListMap["SYLT"].front()->toString().to8Bit(useUnicode);
    //     retTag->lyricsLength = retTag->lyrics.length();
    // }
    if (!frameListMap["USLT"].isEmpty() && frameListMap["USLT"].front() != nullptr) {
        const auto lyricString = frameListMap["USLT"].front()->toString().to8Bit(useUnicode);
        retTag->lyricsLength = lyricString.length();
        retTag->lyrics = lyricString;
    } else {
        retTag->lyrics = "";
        retTag->lyricsLength = 0;
    }
    if (readImage && !frameListMap["APIC"].isEmpty()) {
        auto albumCover = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame *>(frameListMap["APIC"].front());
        if (albumCover != nullptr && albumCover->picture().size() > 0) {
            Util::Picture picture = {};
            picture.size = albumCover->picture().size();
            picture.data.append(albumCover->picture());
            picture.mimetype = albumCover->mimeType().to8Bit(option.useUnicode());
            retTag->albumCover = std::make_unique<Util::Picture>(picture);
        } else {
            retTag->albumCover = std::make_unique<Util::Picture>();
        }
    } else {
        retTag->albumCover = std::make_unique<Util::Picture>();
    }
    return retTag;
}

} // namespace Meipuru
