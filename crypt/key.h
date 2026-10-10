#pragma once
#include <cstdint>


uint8_t* generate_key();
uint8_t* get_child(const uint8_t*&, const uint8_t, const uint8_t);
