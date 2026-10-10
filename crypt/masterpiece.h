#pragma once
#include "key.h"
#include "quads.h"
#include "generator.h"
#include "text.h"


struct LOCAL_MAX {
    uint8_t* key{};
    double score{};
};

LOCAL_MAX local_attempt(const uint8_t*&, const std::vector<uint8_t>&, const std::vector<double>&);
double score(const uint8_t*&, const std::vector<uint8_t>&, const std::vector<double>&);