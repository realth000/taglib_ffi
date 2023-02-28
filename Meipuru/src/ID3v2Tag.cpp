#include "ID3v2Tag.h"

namespace Meipuru {
    void ID3v2Tag::print() {
        BaseTag::print();
        std::cout << "Album Cover (size): " << this->albumCover.size << "\n"
                  << "Lyrics (size): " << lyrics.size() << std::endl;
    }
}