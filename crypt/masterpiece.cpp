#include "masterpiece.h"

LOCAL_MAX local_attempt(const uint8_t* key, std::vector<uint8_t> text) {
    
}

double score(const uint8_t*& key, const std::vector<uint8_t>& text, const std::vector<double>& quads) {
    double score{};
    for(size_t i{}; i < text.size() - 3; ++i) {
        score += quads[encode_quadgram(text[i], text[i + 1], text[i + 2], text[i + 3])];
    }
    return score;
}