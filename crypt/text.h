#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <cstdint>


std::vector<uint8_t> get_cipher_text(std::ifstream&);
std::vector<uint8_t> decrypt(const std::vector<uint8_t>&, const uint8_t*);
std::string to_utf8(const std::vector<uint8_t>&);