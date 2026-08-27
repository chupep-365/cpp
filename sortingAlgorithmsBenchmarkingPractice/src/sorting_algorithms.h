#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>

// глобальный счетчик максимальной глубины рекурсии.
// обнуляется перед запуском каждой сортировки.
// для нерекурсивных сортировок остается равным 0.
extern size_t max_recursion_depth;

// O(n^2) сортировки
void bubble_sort(std::vector<uint32_t>& arr);
void selection_sort(std::vector<uint32_t>& arr);
void insertion_sort(std::vector<uint32_t>& arr);

// O(n^2) сортировки
// варианты реализации QuickSort
// в наивной вариации опорным выбирается последний элемент
// в улучшенной - элемент из средины
void quick_sort_naive(std::vector<uint32_t>& arr);
void quick_sort_improved(std::vector<uint32_t>& arr);
// варианты реализации MergeSort
// в наивной реализации на каждом "шаге" рекурсии создается новый временный массив
// в улучшенной - всего один временный массив
void merge_sort_naive(std::vector<uint32_t>& arr);
void merge_sort_improved(std::vector<uint32_t>& arr);
// варианты реализации HeapSort
// реализованы итеративный и рекурсивный варианты, что отражается на используемой алгоритмом памяти
void heap_sort_iterative(std::vector<uint32_t>& arr);
void heap_sort_recursive(std::vector<uint32_t>& arr);

// O(n) сортировки
void counting_sort(std::vector<uint32_t>& arr);
void radix_sort(std::vector<uint32_t>& arr);
void bucket_sort(std::vector<uint32_t>& arr);

// встроенная сортировка
void builtin_sort(std::vector<uint32_t>& arr);