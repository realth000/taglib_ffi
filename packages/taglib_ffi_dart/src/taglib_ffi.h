#pragma once

/////////
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif

#ifdef _WIN32
#define FFI_PLUGIN_EXPORT __declspec(dllexport)
#else
#define FFI_PLUGIN_EXPORT __attribute__((visibility("default")))
#endif
/////

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  const char *filePath;
  const char *fileName;
  const char *title;
  const char *artist;
  const char *albumTitle;
  const char *albumArtist;
  unsigned int year;
  unsigned int track;
  int albumTotalTrack;
  const char *genre;
  const char *comment;
  int bitRate;
  int sampleRate;
  int channels;
  int lengthInSeconds;
  int lengthInMilliseconds;
  // The `_owner` field is an internal member who really owns the strings in
  // current `Tag`. DO NOT MODIFY it except free;
  void *_owner;
} Tag;

typedef struct {
  const char *filePath;
  const char *fileName;
  const char *title;
  const char *artist;
  const char *albumTitle;
  const char *albumArtist;
  unsigned int year;
  unsigned int track;
  int albumTotalTrack;
  const char *genre;
  const char *comment;
  int bitRate;
  int sampleRate;
  int channels;
  int lengthInSeconds;
  int lengthInMilliSeconds;
  const char *lyrics;
  unsigned long lyricsLength;
  const char *albumCover;
  unsigned int albumCoverLength;
  // The `_owner` field is an internal member who really owns the strings in
  // current `Tag`. DO NOT MODIFY it except free;
  void *_owner;
} ID3v2Tag;

FFI_PLUGIN_EXPORT Tag *readTag(const char *filePath);

FFI_PLUGIN_EXPORT ID3v2Tag *readID3v2Tag(const char *filePath);

FFI_PLUGIN_EXPORT void freeTag(const Tag *tag);

FFI_PLUGIN_EXPORT void freeID3v2Tag(const ID3v2Tag *id3V2Tag);

// Only use for debug.
FFI_PLUGIN_EXPORT void printID3v2Tag(ID3v2Tag *tag);

#ifdef __cplusplus
}
#endif
