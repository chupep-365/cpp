#include "quads.h"

uint32_t encode_quadgram(const uint8_t A, const uint8_t B, const uint8_t C, const uint8_t D) {
    return (((A * ALPHABET_SIZE + B) * ALPHABET_SIZE + C) * ALPHABET_SIZE + D);
}

const std::vector<double> get_quadgrams(std::ifstream& fin) {
    std::string temp{};
    char alpha{};
    std::vector<double> quads(std::pow(ALPHABET_SIZE, 4), -1.0);
    while(fin >> temp) {
        uint8_t a{}, b{}, c{}, d{};
        const char A{temp[0]}, B{temp[1]}, C{temp[2]}, D{temp[3]};
        a = find(A); b = find(B); c = find(C); d = find(D);
        quads[encode_quadgram(a, b, c, d)] = std::log10((double)(atoi(temp.substr(5).c_str())));
    }
    return quads;
}

uint8_t find(const char alpha) {
    for(size_t i{0}; i < ALPHABET_SIZE; ++i) {
        if(alpha == *ALPHABET[i]) {
            return i;
        }
    }
    return -1;
}