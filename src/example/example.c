#include <stdio.h>

#include "../taglib_ffi.h"

int main(int argc, char *argv[]) {
  //
  if (argc < 2) {
    fprintf(stderr, "usage: %s [xxx.mp3]\n", argv[0]);
    return 1;
  }

  printf("reading from %s\n", argv[1]);

  const ID3v2Tag *fp = readID3v2Tag(argv[1]);
  if (!fp) {
    fprintf(stderr, "failed to load file %s", argv[1]);
    return 1;
  }

  printf("title=%s, artist=%s, album=%s\n", fp->title, fp->artist,
         fp->albumTitle);

  return 0;
}
