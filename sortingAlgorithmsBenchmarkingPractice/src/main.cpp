#include <iostream>
#include <vector>
#include <algorithm>
#include "generator.h"
#include "sorting_algorithms.h"

void test_sort(const char* name, void (*sort_func)(std::vector<uint32_t>&)) {
    std::vector<uint32_t> arr = gen_arr(1000, arr_type::RANDOM);
    std::vector<uint32_t> original = arr;
    sort_func(arr);
    bool sorted = std::is_sorted(arr.begin(), arr.end());
    bool same_elements = true;
    std::cout << name << ": " << (sorted ? "OK" : "FAIL") << '\n';
}

int main() {
    test_sort("bubble_sort", bubble_sort);
    test_sort("selection_sort", selection_sort);
    test_sort("insertion_sort", insertion_sort);
    test_sort("quick_sort_naive", quick_sort_naive);
    test_sort("quick_sort_improved", quick_sort_improved);
    test_sort("merge_sort_naive", merge_sort_naive);
    test_sort("merge_sort_improved", merge_sort_improved);
    test_sort("heap_sort_iterative", heap_sort_iterative);
    test_sort("heap_sort_recursive", heap_sort_recursive);
    test_sort("counting_sort", counting_sort);
    test_sort("radix_sort", radix_sort);
    test_sort("bucket_sort", bucket_sort);
    test_sort("builtin_sort", builtin_sort);
    return 0;
}