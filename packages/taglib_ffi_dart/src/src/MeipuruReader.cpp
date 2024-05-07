#include "MeipuruReader.h"

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

bool MeipuruReaderOption::useUnicode() const { return unicode; }

MeipuruReader::MeipuruReader(const MeipuruReaderOption &meipuruReaderOption)
    : option(meipuruReaderOption) {}

BaseTag *MeipuruReader::readTagFromFile(const char *filePath) {
  std::cout << "Reading file" << filePath << std::endl;
  const TagLib::FileRef fileRef(filePath);
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

bool MeipuruReader::fetchBaseTag(const TagLib::File *file,
                                 BaseTag *baseTag) const {
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
  baseTag->albumArtist =
      propertyMap["ALBUMARTIST"].toString().to8Bit(useUnicode);
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
    baseTag->lengthInSeconds = audioProperties->lengthInSeconds();
    baseTag->lengthInMilliseconds = audioProperties->lengthInMilliseconds();
  } else {
    baseTag->bitRate = 0;
    baseTag->sampleRate = 0;
    baseTag->channels = 0;
    baseTag->lengthInSeconds = 0;
    baseTag->lengthInMilliseconds = 0;
  }
  return true;
}

ID3v2Tag *MeipuruReader::readID3v2TagFromFile(const char *filePath) {
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
  fetchBaseTag(&mpegFile, retTag);
  const auto &frameListMap = id3v2Tag->frameListMap();
  // TODO: Handle synchronous lyrics.
  // if (!frameListMap["SYLT"].isEmpty() && frameListMap["SYLT"].front() !=
  // nullptr) {
  //     retTag->lyrics =
  //     frameListMap["SYLT"].front()->toString().to8Bit(useUnicode);
  //     retTag->lyricsLength = retTag->lyrics.length();
  // }
  if (!frameListMap["USLT"].isEmpty() &&
      frameListMap["USLT"].front() != nullptr) {
    const auto lyricString =
        frameListMap["USLT"].front()->toString().to8Bit(useUnicode);
    retTag->lyricsLength = lyricString.length();
    retTag->lyrics = lyricString;
  } else {
    retTag->lyrics = "";
    retTag->lyricsLength = 0;
  }
  if (!frameListMap["APIC"].isEmpty()) {
    auto albumCover = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame *>(
        frameListMap["APIC"].front());
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
