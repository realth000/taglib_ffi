#pragma once

/////////
// #ifdef _WIN32
// #include <windows.h>
// #else
// #include <pthread.h>
// #include <unistd.h>
// #endif

#include <cstddef>

#ifdef _WIN32
#define FFI_PLUGIN_EXPORT __declspec(dllexport)
#else
#define FFI_PLUGIN_EXPORT __attribute__((visibility("default")))
#endif
/////

#ifdef __cplusplus
extern "C" {
#endif

struct Picture {
    char *data;
    unsigned int size;
    char *mimeType;
};

typedef struct Id3v2TagOwner Id3v2TagOwner;

// ID3v2
struct ID3v2Tag {
    const char *filePath;
    const char *fileName;
    const char *title;
    const char **artist;
    size_t artistLength;
    const char *albumTitle;
    const char **albumArtist;
    size_t albumArtistLength;
    unsigned int year;
    unsigned int track;
    unsigned int albumTotalTrack;
    const char *genre;
    const char *comment;
    unsigned int bitRate;
    unsigned int sampleRate;
    unsigned int channels;
    unsigned int lengthInSeconds;
    unsigned int lengthInMilliseconds;
    const Picture *picture;
    size_t pictureLength;
    const char *lyrics;

    // Internal opaque type.
    const Id3v2TagOwner *_owner;
};
FFI_PLUGIN_EXPORT const ID3v2Tag *readID3v2Tag(const char *filePath, bool readImage);
FFI_PLUGIN_EXPORT void freeID3v2Tag(const ID3v2Tag *tag);

// Only use for debug.
// FFI_PLUGIN_EXPORT void printID3v2Tag(ID3v2Tag *tag);

#ifdef __cplusplus
}
#endif
