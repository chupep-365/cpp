#pragma once 
#include <vector>
#include <cstdint>


enum class arr_type { RANDOM, SORTED, REVERSE, ALMOST_SORTED };

// генерирует члучайное число в диапазоне от min до max
uint32_t rand_int(uint32_t min, uint32_t max);

// функция которая генерирует входные данные - массив заданного типа
std::vector<uint32_t> gen_arr(size_t size, arr_type type);

// вспомогательная функция для генерации рандомного массива - тасование Фишера-Йетса
void shuffle(std::vector<uint32_t>& arr);