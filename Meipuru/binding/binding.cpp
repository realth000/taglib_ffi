#include "../include/binding.h"

#include <cstring>

#include "../include/MeipuruReader.h"

MeipuruTag *MeipuruReadTag(const char *filePath) {
  auto reader = Meipuru::MeipuruReader();
  auto tag = reader.readTagFromFile(filePath);

  auto *meipuruTag = new MeipuruTag{
      strdup(tag->filePath.c_str()),
      strdup(tag->fileName.c_str()),
      strdup(tag->title.c_str()),
      strdup(tag->artist.c_str()),
      strdup(tag->albumTitle.c_str()),
      strdup(tag->albumArtist.c_str()),
      tag->year,
      tag->track,
      tag->albumTotalTrack,
      strdup(tag->genre.c_str()),
      strdup(tag->comment.c_str()),
      tag->bitRate,
      tag->sampleRate,
      tag->channels,
      tag->length,
  };
  delete tag;
  return meipuruTag;
}

MeipuruID3v2Tag *MeipuruReadID3v2Tag(const char *filePath) {
  auto reader = Meipuru::MeipuruReader();
  auto id3v2Tag = reader.readID3v2TagFromFile(filePath);

  if (id3v2Tag == nullptr) {
    return nullptr;
  }

  auto *meipuruID3v2Tag = new MeipuruID3v2Tag{
      strdup(id3v2Tag->filePath.c_str()),
      strdup(id3v2Tag->fileName.c_str()),
      strdup(id3v2Tag->title.c_str()),
      strdup(id3v2Tag->artist.c_str()),
      strdup(id3v2Tag->albumTitle.c_str()),
      strdup(id3v2Tag->albumArtist.c_str()),
      id3v2Tag->year,
      id3v2Tag->track,
      id3v2Tag->albumTotalTrack,
      strdup(id3v2Tag->genre.c_str()),
      strdup(id3v2Tag->comment.c_str()),
      id3v2Tag->bitRate,
      id3v2Tag->sampleRate,
      id3v2Tag->channels,
      id3v2Tag->length,
      strdup(id3v2Tag->lyrics.c_str()),
      (unsigned long)(id3v2Tag->lyrics.size()),
      id3v2Tag->albumCover.data,
      id3v2Tag->albumCover.size,
  };
  delete id3v2Tag;
  return meipuruID3v2Tag;
}

MeipuruID3v2Tag *MeipuruReadID3v2TagW(const wchar_t *filePath) {
  auto reader = Meipuru::MeipuruReader();
  auto id3v2Tag = reader.readID3v2TagFromFileW(filePath);

  if (id3v2Tag == nullptr) {
    return nullptr;
  }

  auto *meipuruID3v2Tag = new MeipuruID3v2Tag{
      strdup(id3v2Tag->filePath.c_str()),
      strdup(id3v2Tag->fileName.c_str()),
      strdup(id3v2Tag->title.c_str()),
      strdup(id3v2Tag->artist.c_str()),
      strdup(id3v2Tag->albumTitle.c_str()),
      strdup(id3v2Tag->albumArtist.c_str()),
      id3v2Tag->year,
      id3v2Tag->track,
      id3v2Tag->albumTotalTrack,
      strdup(id3v2Tag->genre.c_str()),
      strdup(id3v2Tag->comment.c_str()),
      id3v2Tag->bitRate,
      id3v2Tag->sampleRate,
      id3v2Tag->channels,
      id3v2Tag->length,
      strdup(id3v2Tag->lyrics.c_str()),
      (unsigned long)(id3v2Tag->lyrics.size()),
      id3v2Tag->albumCover.data,
      id3v2Tag->albumCover.size,
  };
  delete id3v2Tag;
  return meipuruID3v2Tag;
}

void MeipuruFreeTag(MeipuruTag *tag) {
  if (tag == nullptr) {
    return;
  }
  // No custom destructor due to used by dart ffi, free manually.
  free(tag->filePath);
  free(tag->fileName);
  free(tag->title);
  free(tag->artist);
  free(tag->albumTitle);
  free(tag->albumArtist);
  free(tag->genre);
  free(tag->comment);
  delete tag;
}

void MeipuruFreeID3v2Tag(MeipuruID3v2Tag *id3V2Tag) {
  if (id3V2Tag == nullptr) {
    return;
  }
  // No custom destructor due to used by dart ffi, free manually.
  free(id3V2Tag->filePath);
  free(id3V2Tag->fileName);
  free(id3V2Tag->title);
  free(id3V2Tag->artist);
  free(id3V2Tag->albumTitle);
  free(id3V2Tag->albumArtist);
  free(id3V2Tag->genre);
  free(id3V2Tag->comment);
  free(id3V2Tag->lyrics);
  delete id3V2Tag;
}
