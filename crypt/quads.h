#pragma once
#include <vector>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <string>

constexpr uint8_t ALPHABET_SIZE = 33;
constexpr const char* ALPHABET[ALPHABET_SIZE]= {
    "\xD0\x90", "\xD0\x91", "\xD0\x92", "\xD0\x93", "\xD0\x94", "\xD0\x95", "\xD0\x81", "\xD0\x96", "\xD0\x97", 
    "\xD0\x98", "\xD0\x99", "\xD0\x9A", "\xD0\x9B", "\xD0\x9C", "\xD0\x9D", "\xD0\x9E", "\xD0\x9F", "\xD0\xA0", 
    "\xD0\xA1", "\xD0\xA2", "\xD0\xA3", "\xD0\xA4", "\xD0\xA5", "\xD0\xA6", "\xD0\xA7", "\xD0\xA8", "\xD0\xA9", 
    "\xD0\xAA", "\xD0\xAB", "\xD0\xAC", "\xD0\xAD", "\xD0\xAE", "\xD0\xAF"  
};

uint32_t encode_quadgram(const uint8_t, const uint8_t, const uint8_t, const uint8_t);
const std::vector<double> get_quadgrams(std::ifstream&);
uint8_t find(const char);
