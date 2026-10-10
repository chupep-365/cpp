#include "key.h"
#include "quads.h"
#include "generator.h"

uint8_t* generate_key() {
    uint8_t* key = new uint8_t[ALPHABET_SIZE];
    for(size_t i{}; i < ALPHABET_SIZE; ++i) {
        key[i] = i;
    }
    shuffle(key, ALPHABET_SIZE);
    return key;
}

uint8_t* get_child(const uint8_t*& parent, const uint8_t a, const uint8_t b) {
    uint8_t* child = new uint8_t[ALPHABET_SIZE];
    for(size_t i{}; i < ALPHABET_SIZE; ++i) {
        child[i] = parent[i];
    }
    std::swap(child[a], child[b]);
    return child;
}
