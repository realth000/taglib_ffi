#include "include/MeipuruReader.h"

#include <cstring>

#include "taglib/taglib/mpeg/id3v2/frames/attachedpictureframe.h"
#include "taglib/taglib/mpeg/id3v2/id3v2tag.h"
#include "taglib/taglib/toolkit/tpropertymap.h"
#include "taglib/taglib/toolkit/tstring.h"

namespace Meipuru {
    MeipuruReaderOption::MeipuruReaderOption() {
        encoding = Util::Encoding::Unicode;
        logLevel = Util::LogLevel::Error;
        unicode = true;
    }

    bool MeipuruReaderOption::useUnicode() const {
        return unicode;
    }

    MeipuruReader::MeipuruReader(const MeipuruReaderOption &meipuruReaderOption) : option(meipuruReaderOption) {
    }

    BaseTag *MeipuruReader::readTagFromFile(const std::string &filePath) {
        std::cout << "Reading file" << filePath << std::endl;
        const TagLib::FileRef fileRef(filePath.c_str());
        if (fileRef.isNull()) {
            std::cout << "File is NULL:" << filePath << std::endl;
            return nullptr;
        }

        auto baseTag = new BaseTag();
        baseTag->filePath = filePath;
        baseTag->fileName = fileRef.file()->name();
        fetchBaseTag(fileRef.file(), baseTag);
        if (option.logLevel == Util::LogLevel::Debug) {
            std::cout << "Read tag from file:" << std::endl;
            baseTag->print();
        }
        return baseTag;
    }

    BaseTag *MeipuruReader::readTagFromFileW(const std::wstring &filePath) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
        const TagLib::FileRef fileRef(filePath.c_str());
        if (fileRef.isNull()) {
            std::cout << "File is NULL:" << filePath << std::endl;
            return nullptr;
        }

        auto baseTag = new BaseTag();
        // Do not overwrite file path if using WChar_t in other words UTF-16.
        // baseTag->filePath = filePath.;
        baseTag->fileName = fileRef.file()->name();
        fetchBaseTag(fileRef.file(), baseTag);
        if (option.logLevel == Util::LogLevel::Debug) {
            std::cout << "Read tag from file:" << std::endl;
            baseTag->print();
        }
        return baseTag;
#else
		return nullptr;
#endif
    }

    bool MeipuruReader::fetchBaseTag(const TagLib::File *file, BaseTag *baseTag) const {
        const auto *tag = file->tag();
        if (tag == nullptr) {
            return false;
        }
        const bool useUnicode = option.useUnicode();
        baseTag->title = tag->title().to8Bit(useUnicode);
        baseTag->artist = tag->artist().to8Bit(useUnicode);
        baseTag->albumTitle = tag->album().to8Bit(useUnicode);
        baseTag->year = tag->year();
        baseTag->track = tag->track();
        baseTag->genre = tag->genre().to8Bit(useUnicode);
        baseTag->comment = tag->comment().to8Bit(useUnicode);

        const auto propertyMap = file->properties();
        // TODO: albumArtist is TagLib::string, should be std::string types.
        baseTag->albumArtist = propertyMap["ALBUMARTIST"].toString().to8Bit(useUnicode);
        const auto trackNumberString = propertyMap["TRACKNUMBER"].toString();
        if (!trackNumberString.isEmpty()) {
            const auto pos = trackNumberString.split("/");
            if (pos.size() == 2) {
                // "1/20"
                baseTag->track = pos[0].toInt();
                baseTag->albumTotalTrack = pos[1].toInt();
            } else if (trackNumberString[0] == '/') {
                // "/20"
                baseTag->albumTotalTrack = pos[1].toInt();
            } else {
                // "1"
                baseTag->track = trackNumberString.toInt();
            }
        }
        const auto *audioProperties = file->audioProperties();
        if (audioProperties != nullptr) {
            baseTag->bitRate = audioProperties->bitrate();
            baseTag->sampleRate = audioProperties->sampleRate();
            baseTag->channels = audioProperties->channels();
            baseTag->length = audioProperties->length();
        } else {
            baseTag->bitRate = 0;
            baseTag->sampleRate = 0;
            baseTag->channels = 0;
            baseTag->length = 0;
        }
        return true;
    }

    ID3v2Tag *MeipuruReader::readID3v2TagFromFile(const std::string &filePath) {
        TagLib::MPEG::File mpegFile(filePath.c_str());
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
        fetchBaseTag(&mpegFile, retTag);
        const auto frameListMap = id3v2Tag->frameListMap();
        // TODO: Handle synchronous lyrics.
        // if (!frameListMap["SYLT"].isEmpty() && frameListMap["SYLT"].front() != nullptr) {
        //     retTag->lyrics = frameListMap["SYLT"].front()->toString().to8Bit(useUnicode);
        //     retTag->lyricsLength = retTag->lyrics.length();
        // }
        if (!frameListMap["USLT"].isEmpty() && frameListMap["USLT"].front() != nullptr) {
            retTag->lyrics = frameListMap["USLT"].front()->toString().to8Bit(useUnicode);
            retTag->lyricsLength = retTag->lyrics.length();
        } else {
            retTag->lyrics = "";
        }
        if (!frameListMap["APIC"].isEmpty()) {
            auto albumCover = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame *>(frameListMap["APIC"].front());
            if (albumCover != nullptr && albumCover->picture().size() > 0) {
                retTag->albumCover.size = albumCover->picture().size();
                retTag->albumCover.data = (char *) malloc(sizeof(char) * retTag->albumCover.size + 1);
                memcpy(retTag->albumCover.data, albumCover->picture().data(), retTag->albumCover.size);
                retTag->albumCover.data[retTag->albumCover.size] = '\0';
                retTag->albumCover.mimetype = albumCover->mimeType().to8Bit(option.useUnicode());
                // std::cout << "Album Cover: YES" << albumCover->mimeType() << std::endl;
                // std::fstream picStream;
                // picStream.open("./test.jpg", std::ios::out | std::ios::binary | std::ios::trunc);
                // picStream.write(albumCover->picture().data(), albumCover->picture().size());
                // picStream.close();
            } else {
                retTag->albumCover.data = nullptr;
                retTag->albumCover.size = 0;
                retTag->albumCover.mimetype = "";
            }
        } else {
            retTag->albumCover.data = nullptr;
            retTag->albumCover.size = 0;
            retTag->albumCover.mimetype = "";
        }
        return retTag;
    }
    ID3v2Tag *MeipuruReader::readID3v2TagFromFileW(const std::wstring &filePath) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
        TagLib::MPEG::File mpegFile(filePath.c_str());
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
        // Do not overwrite file path if using WChar_t in other words UTF-16.
        // retTag->filePath = filePath;
        retTag->fileName = mpegFile.name();
        fetchBaseTag(&mpegFile, retTag);
        const auto frameListMap = id3v2Tag->frameListMap();
        // TODO: Handle synchronous lyrics.
        // if (!frameListMap["SYLT"].isEmpty() && frameListMap["SYLY"].front() != nullptr) {
        //     retTag->lyrics = frameListMap["SYLT"].front()->toString().to8Bit(useUnicode);
        //     retTag->lyricsLength = retTag->lyrics.length();
        // }
        if (!frameListMap["USLT"].isEmpty() && frameListMap["USLT"].front() != nullptr) {
            retTag->lyrics = frameListMap["USLT"].front()->toString().to8Bit(useUnicode);
            retTag->lyricsLength = retTag->lyrics.length();
        } else {
            retTag->lyrics = "";
        }
        if (!frameListMap["APIC"].isEmpty()) {
            auto albumCover = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame *>(frameListMap["APIC"].front());
            if (albumCover != nullptr && albumCover->picture().size() > 0) {
                retTag->albumCover.size = albumCover->picture().size();
                retTag->albumCover.data = (char *) malloc(sizeof(char) * retTag->albumCover.size + 1);
                memcpy(retTag->albumCover.data, albumCover->picture().data(), retTag->albumCover.size);
                retTag->albumCover.data[retTag->albumCover.size] = '\0';
                retTag->albumCover.mimetype = albumCover->mimeType().to8Bit(option.useUnicode());
                // std::cout << "Album Cover: YES" << albumCover->mimeType() << std::endl;
                // std::fstream picStream;
                // picStream.open("./test.jpg", std::ios::out | std::ios::binary | std::ios::trunc);
                // picStream.write(albumCover->picture().data(), albumCover->picture().size());
                // picStream.close();
            } else {
                retTag->albumCover.data = nullptr;
                retTag->albumCover.size = 0;
                retTag->albumCover.mimetype = "";
            }
        } else {
            retTag->albumCover.data = nullptr;
            retTag->albumCover.size = 0;
            retTag->albumCover.mimetype = "";
        }
        return retTag;
#else
		return nullptr;
#endif
    }

}// namespace Meipuru
