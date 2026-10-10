#include "generator.h"


uint8_t rand_int(uint8_t min, uint8_t max) {
    static std::mt19937 gen(std::random_device{}()); 
    std::uniform_int_distribution<uint32_t> dstrb(min, max);
    return dstrb(gen);
}

void shuffle(uint8_t*& arr, uint8_t size) {
    for(size_t i{0}; i < size - 1; ++i) {
        std::swap(arr[i], arr[rand_int(i, size - 1)]);
    }
}