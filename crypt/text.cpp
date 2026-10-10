#include "text.h"
#include "quads.h"

std::vector<uint8_t> get_cipher_text(std::ifstream& fin) {
    std::string temp;
    fin >> temp;
    std::vector<uint8_t> id_arr{};
    for(size_t i{}; i < temp.size(); ++i) {
        uint8_t id = find(temp[i]);
        id_arr.push_back(id);
    }
    fin.close();
    return id_arr;
}

std::vector<uint8_t> decrypt(const std::vector<uint8_t>& text, const uint8_t* key) {
    std::vector<uint8_t> plain;
    for(size_t i{}; i < text.size(); ++i) {
        plain[i] = key[text[i]];
    }
    return plain;
}