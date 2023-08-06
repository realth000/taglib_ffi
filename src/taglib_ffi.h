#pragma once

/////////
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if _WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif

#if _WIN32
#define FFI_PLUGIN_EXPORT __declspec(dllexport)
#else
#define FFI_PLUGIN_EXPORT
#endif
/////

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  char *filePath;
  char *fileName;
  char *title;
  char *artist;
  char *albumTitle;
  char *albumArtist;
  unsigned int year;
  unsigned int track;
  int albumTotalTrack;
  char *genre;
  char *comment;
  int bitRate;
  int sampleRate;
  int channels;
  int length;
} Tag;

typedef struct {
  char *filePath;
  char *fileName;
  char *title;
  char *artist;
  char *albumTitle;
  char *albumArtist;
  unsigned int year;
  unsigned int track;
  int albumTotalTrack;
  char *genre;
  char *comment;
  int bitRate;
  int sampleRate;
  int channels;
  int length;
  char *lyrics;
  unsigned long lyricsLength;
  char *albumCover;
  unsigned int albumCoverLength;
} ID3v2Tag;

#ifdef _WIN32
FFI_PLUGIN_EXPORT Tag *readTag(const wchar_t *filePath);
#else
FFI_PLUGIN_EXPORT Tag *readTag(const char *filePath);
#endif

#ifdef _WIN32
FFI_PLUGIN_EXPORT ID3v2Tag *ReadID3v2Tag(const wchar_t *filePath);
#else
FFI_PLUGIN_EXPORT ID3v2Tag *readID3v2Tag(const char *filePath);
#endif

FFI_PLUGIN_EXPORT void freeTag(Tag *tag);

FFI_PLUGIN_EXPORT void freeID3v2Tag(ID3v2Tag *id3V2Tag);

#ifdef __cplusplus
}

#define MEIPURU_EXPORT FFI_PLUGIN_EXPORT
#endif
