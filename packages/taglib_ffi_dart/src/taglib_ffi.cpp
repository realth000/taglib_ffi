#include "taglib_ffi.h"

#include "src/MeipuruReader.h"

FFI_PLUGIN_EXPORT Tag *readTag(const char *filePath) {
  auto reader = Meipuru::MeipuruReader();

  auto tag = reader.readTagFromFile(filePath);

  // The data in `tag` is also implicitly "borrowed" by `meipuruTag`.
  auto *meipuruTag = new Tag{
      tag->filePath.c_str(),
      tag->fileName.c_str(),
      tag->title.c_str(),
      tag->artist.c_str(),
      tag->albumTitle.c_str(),
      tag->albumArtist.c_str(),
      tag->year,
      tag->track,
      tag->albumTotalTrack,
      tag->genre.c_str(),
      tag->comment.c_str(),
      tag->bitRate,
      tag->sampleRate,
      tag->channels,
      tag->lengthInSeconds,
      tag->lengthInMilliseconds,
      tag,
  };
  return meipuruTag;
}

FFI_PLUGIN_EXPORT ID3v2Tag *readID3v2Tag(const char *filePath, bool readImage) {
  auto reader = Meipuru::MeipuruReader();
  auto id3v2Tag = reader.readID3v2TagFromFile(filePath, readImage);

  if (id3v2Tag == nullptr) {
    return nullptr;
  }

  // The data in `id3v2Tag` is also implicitly "borrowed" by
  // `meipuruID3v2Tag`.
  auto *meipuruID3v2Tag = new ID3v2Tag{
      id3v2Tag->filePath.c_str(),
      id3v2Tag->fileName.c_str(),
      id3v2Tag->title.c_str(),
      id3v2Tag->artist.c_str(),
      id3v2Tag->albumTitle.c_str(),
      id3v2Tag->albumArtist.c_str(),
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
      id3v2Tag->lyrics.c_str(),
      (unsigned long)(id3v2Tag->lyricsLength),
      nullptr,
      0,
      id3v2Tag,
  };
  if (readImage) {
    meipuruID3v2Tag->albumCover = (uint8_t *)id3v2Tag->albumCover->data.data();
    meipuruID3v2Tag->albumCoverLength = id3v2Tag->albumCover->size;
  }
  return meipuruID3v2Tag;
}

FFI_PLUGIN_EXPORT void freeTag(const Tag *tag) {
  delete static_cast<Meipuru::BaseTag *>(tag->_owner);
  delete tag;
}

FFI_PLUGIN_EXPORT void freeID3v2Tag(const ID3v2Tag *id3V2Tag) {
  delete static_cast<Meipuru::ID3v2Tag *>(id3V2Tag->_owner);
  delete id3V2Tag;
}

FFI_PLUGIN_EXPORT void printID3v2Tag(ID3v2Tag *id3V2Tag) {
  if (id3V2Tag == nullptr) {
    std::cout << "ptr is empty" << std::endl;
    return;
  }

  std::cout << "File: " << id3V2Tag->fileName << "\n"
            << "File Path: " << id3V2Tag->filePath << "\n"
            << "Title: " << id3V2Tag->title << "\n"
            << "Artist: " << id3V2Tag->artist << "\n"
            << "Album Title: " << id3V2Tag->albumTitle << "\n"
            << "Album Artist: " << id3V2Tag->albumArtist << "\n"
            << "Year: " << id3V2Tag->year << "\n"
            << "Track: " << id3V2Tag->track << "\n"
            << "Album Track Count: " << id3V2Tag->albumTotalTrack << "\n"
            << "Genre: " << id3V2Tag->genre << "\n"
            << "Comment: " << id3V2Tag->comment << "\n"
            << "Bit Rate: " << id3V2Tag->bitRate << "\n"
            << "Sample Rate: " << id3V2Tag->sampleRate << "\n"
            << "Channels: " << id3V2Tag->channels << "\n"
            << "LengthInSeconds: " << id3V2Tag->lengthInSeconds << "\n"
            << "LengthInMilliseconds: " << id3V2Tag->lengthInMilliSeconds
            << "\n"
            << "Album Cover (size): " << id3V2Tag->albumCoverLength << "\n"
            << "Lyrics (size): " << id3V2Tag->lyricsLength << std::endl;
}
