#pragma once
#include <cstddef>

// сбрасывает счетчики перед началом замера
void reset_memory_tracker();
// возвращает пиковое потребление памяти в байтах
size_t get_peak_memory_bytes();