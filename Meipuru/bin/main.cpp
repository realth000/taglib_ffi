#include <fstream>
#include <iostream>

#include "include/MeipuruReader.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " [audio file]";
        return 0;
    }
    auto meipuruReader = Meipuru::MeipuruReader();
#if 1
    for (int i = 1; i < argc; i++) {
        auto id3v2Tag = meipuruReader.readID3v2TagFromFile(argv[i]);
        id3v2Tag->print();
        delete id3v2Tag;
    }
#else
    auto baseTag = meipuruReader.readTagFromFile(argv[1]);
    baseTag->print();
    delete baseTag;
#endif
    return 0;

    for (int i = 1; i < argc; i++) {
        /*
         * Consider files have id3V2 tags, use specified methods to fetch more data.
         */
        // Second parameter (type bool) controls read file's audio properties or not.

    }
    return 0;
}
