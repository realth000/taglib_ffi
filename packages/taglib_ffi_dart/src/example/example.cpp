#include <filesystem>
#include <iostream>

#include "../taglib_ffi.h"

namespace fs = std::filesystem;

void printFileMetadata(std::string_view filePath) {
  const ID3v2Tag *fp = readID3v2Tag(filePath.data());
  if (!fp) {
    std::cout << "failed to read " << filePath << std::endl;
    return;
  }

  printf("title=%s, artist=%s, album=%s\n", fp->title, fp->artist,
         fp->albumTitle);

  freeID3v2Tag(fp);
}

void walk(std::string_view path) {
  if (fs::is_regular_file(path) && fs::path(path).extension() == ".mp3") {
    printFileMetadata(path);
  } else if (fs::is_directory(path)) {
    for (const auto &entry : fs::directory_iterator(path)) {
      walk(entry.path().string());
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "usage: %s [xxx.mp3]" << argv[0] << std::endl;
    return 1;
  }

  const std::string rootPath = {argv[1]};

  if (!fs::exists(rootPath)) {
    std::cerr << rootPath << " not exists" << std::endl;
    return 1;
  }

  walk(rootPath);

  return 0;
}
