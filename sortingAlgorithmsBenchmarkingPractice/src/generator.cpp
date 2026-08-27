#include "generator.h"
#include <random>
#include <algorithm> 

uint32_t rand_int(uint32_t min, uint32_t max) {
    static std::mt19937 gen( std::random_device {}());
    std::uniform_int_distribution<uint32_t> dstrb(min, max);
    return dstrb(gen);
}

std::vector<uint32_t> gen_arr(size_t size, arr_type type) {
    std::vector<uint32_t> arr{};
    for(size_t i{0}; i < size; ++i)
    {
        arr.push_back(i);
    }
    switch (type)
    {
    case arr_type::SORTED:
        return arr;
    case arr_type::REVERSE:
        for(size_t i{size - 1}; i > (size - 1) / 2; --i){
            std::swap(arr[i], arr[size - i - 1]);
        }
        return arr;
    case arr_type::ALMOST_SORTED:
        for(size_t i{0}; i < (size_t)(std::ceil(size * 0.05)); ++i) {
            std::swap(arr[rand_int(0, size - 1)], arr[rand_int(0, size - 1)]);
        }
        return arr;
    case arr_type::RANDOM:
        shuffle(arr);
        return arr;
    default:
        throw "###   WRONG ARR_TYPE!  ###";
    }
}

void shuffle(std::vector<uint32_t>& arr) {
    const size_t size = arr.size();
    if(size < 2) {
        return;
    }
    for(size_t i{0}; i + 1 < size; ++i) {
        std::swap(arr[i], arr[rand_int(i, size - 1)]);
    }
}