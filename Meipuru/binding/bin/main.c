#include "include/binding.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s [audio file]", argv[0]);
        return 0;
    }
    MeipuruID3v2Tag *id3V2Tag = MeipuruReadID3v2Tag(argv[1]);
    printf("File: %s\n"
           "File Path: %s\n"
           "Title: %s\n"
           "Artist: %s\n"
           "Album Title: %s\n"
           "Album Artist: %s\n"
           "Year: %u\n"
           "Track: %u\n"
           "Album Track Count: %d\n"
           "Genre: %s\n"
           "Comment: %s\n"
           "Bit Rate: %d\n"
           "Sample Rate: %d\n"
           "Channels: %d\n"
           "Length: %d\n"
           "Lyrics (length): %zd\n",
           id3V2Tag->fileName,
           id3V2Tag->filePath,
           id3V2Tag->title,
           id3V2Tag->artist,
           id3V2Tag->albumTitle,
           id3V2Tag->albumArtist,
           id3V2Tag->year,
           id3V2Tag->track,
           id3V2Tag->albumTotalTrack,
           id3V2Tag->genre,
           id3V2Tag->comment,
           id3V2Tag->bitRate,
           id3V2Tag->sampleRate,
           id3V2Tag->channels,
           id3V2Tag->length,
           strlen(id3V2Tag->lyrics));
    if (id3V2Tag->albumCover != NULL && id3V2Tag->albumCoverLength > 0) {
        printf("Album Cover (size): %d\n", id3V2Tag->albumCoverLength);
    }
    MeipuruFreeID3v2Tag(id3V2Tag);
    return 0;
}
