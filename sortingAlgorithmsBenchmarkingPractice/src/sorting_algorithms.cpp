#include "sorting_algorithms.h"

#include <algorithm>
#include <utility>
#include <vector>
#include <cstdint>
#include <cstddef>

size_t max_recursion_depth = 0;

// BubbleSort
void bubble_sort(std::vector<uint32_t>& arr) {
    max_recursion_depth = 0;
    if(arr.size() < 2) {
        return;
    }
    const size_t size = arr.size();
    for(size_t i{0}; i < size - 1; ++i) {
        bool swapped = false;
        for(size_t j{0}; j < size - 1 - i; ++j) {
            if(arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if(!swapped) {
            break;
        }
    }
}

// SelectionSort
void selection_sort(std::vector<uint32_t>& arr) {
    max_recursion_depth = 0;
    if(arr.size() < 2) {
        return;
    }
    const size_t size = arr.size();
    for(size_t i{0}; i < size - 1; ++i) {
        size_t min_index = i;
        for(size_t j ={i + 1}; j < size; ++j) {
            if (arr[j] < arr[min_index]) {
                min_index = j;
            }
        }
        if(min_index != i) {
            std::swap(arr[i], arr[min_index]);
        }
    }
}

// InsertionSort
void insertion_sort(std::vector<uint32_t>& arr) {
    max_recursion_depth = 0;
    if(arr.size() < 2) {
        return;
    }
    const size_t size = arr.size();
    for(size_t i{1}; i < size; ++i) {
        const uint32_t key = arr[i];
        size_t j = i;
        while(j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
}

// QuickSort
size_t partition_quicksort_naive(std::vector<uint32_t>& arr, size_t low, size_t high) {
    const uint32_t pivot = arr[high];
    size_t i = low;
    for(size_t j{low}; j < high; ++j) {
        if(arr[j] <= pivot) {
            std::swap(arr[i], arr[j]);
            ++i;
        }
    }
    std::swap(arr[i], arr[high]);
    return i;
}

// специально обрабатывается меньшая часть чтобы избежать stack overflow при n > 50000
// при этом сохранена деградация временной сложности до n^2
void quicksort_naive_recursion(std::vector<uint32_t>& arr, size_t low, size_t high, size_t depth) {
    while(low < high) {
        if(depth > max_recursion_depth) {
            max_recursion_depth = depth;
        }
        const size_t pivot_index = partition_quicksort_naive(arr, low, high);
        size_t left_size = pivot_index - low;
        size_t right_size = high - pivot_index;
        if(left_size < right_size) {
            if(pivot_index > low) {
                quicksort_naive_recursion(arr, low, pivot_index - 1, depth + 1);
            }
            low = pivot_index + 1;
        } else {
            if(pivot_index < high) {
                quicksort_naive_recursion(arr, pivot_index + 1, high, depth + 1);
            }
            high = pivot_index - 1;
        }
    }
}

size_t partition_quicksort_improved(std::vector<uint32_t>& arr, size_t low, size_t high) {
    const size_t mid = low + (high - low) / 2;
    std::swap(arr[mid], arr[high]);
    const uint32_t pivot = arr[high];
    size_t i = low;
    for(size_t j{low}; j < high; ++j) {
        if(arr[j] <= pivot) {
            std::swap(arr[i], arr[j]);
            ++i;
        }
    }
    std::swap(arr[i], arr[high]);
    return i;
}

void quicksort_improved_recursion(std::vector<uint32_t>& arr, size_t low, size_t high, size_t depth) {
    if(depth > max_recursion_depth) {
        max_recursion_depth = depth;
    }
    if(low >= high) {
        return;
    }
    const size_t pivot_index = partition_quicksort_improved(arr, low, high);
    if(pivot_index > low) {
        quicksort_improved_recursion(arr, low, pivot_index - 1, depth + 1);
    }
    if(pivot_index < high) {
        quicksort_improved_recursion(arr, pivot_index + 1, high, depth + 1);
    }
}

// QuickSort wrappers
void quick_sort_naive(std::vector<uint32_t>& arr) {
    max_recursion_depth = 0;
    if(arr.size() < 2) {
        return;
    }
    quicksort_naive_recursion(arr, 0, arr.size() - 1, 1);
}

void quick_sort_improved(std::vector<uint32_t>& arr) {
    max_recursion_depth = 0;
    if(arr.size() < 2) {
        return;
    }
    quicksort_improved_recursion(arr, 0, arr.size() - 1, 1);
}

// MergeSort 
void mergesort_naive_recursion(std::vector<uint32_t>& arr, size_t left, size_t right, size_t depth) {
    if(depth > max_recursion_depth) {
        max_recursion_depth = depth;
    }
    if(left >= right) {
        return;
    }
    const size_t segment_size = right - left + 1;
    std::vector<uint32_t> tmp(segment_size);
    const size_t mid = left + (right - left) / 2;
    mergesort_naive_recursion(arr, left, mid, depth + 1);
    mergesort_naive_recursion(arr, mid + 1, right, depth + 1);
    size_t i = left;
    size_t j = mid + 1;
    size_t k = 0;
    while(i <= mid && j <= right) {
        if(arr[i] <= arr[j]) {
            tmp[k] = arr[i];
            ++i;
        } else {
            tmp[k] = arr[j];
            ++j;
        }
        ++k;
    }
    while(i <= mid) {
        tmp[k] = arr[i];
        ++i;
        ++k;
    }
    while(j <= right) {
        tmp[k] = arr[j];
        ++j;
        ++k;
    }
    for(size_t q{0}; q < segment_size; ++q) {
        arr[left + q] = tmp[q];
    }
}

void merge_arrays(std::vector<uint32_t>& arr, size_t left, size_t mid, size_t right,
                  std::vector<uint32_t>& tmp) {
    size_t i = left;
    size_t j = mid + 1;
    size_t k = left;
    while(i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            tmp[k] = arr[i];
            ++i;
        } else {
            tmp[k] = arr[j];
            ++j;
        }
        ++k;
    }
    while(i <= mid) {
        tmp[k] = arr[i];
        ++i;
        ++k;
    }
    while(j <= right) {
        tmp[k] = arr[j];
        ++j;
        ++k;
    }
    for(size_t q{left}; q <= right; ++q) {
        arr[q] = tmp[q];
    }
}

void mergesort_improved_recursion(std::vector<uint32_t>& arr, size_t left, size_t right,
                                   std::vector<uint32_t>& tmp, size_t depth) {
    if(depth > max_recursion_depth) {
        max_recursion_depth = depth;
    }
    if(left >= right) {
        return;
    }
    const size_t mid = left + (right - left) / 2;
    mergesort_improved_recursion(arr, left, mid, tmp, depth + 1);
    mergesort_improved_recursion(arr, mid + 1, right, tmp, depth + 1);
    merge_arrays(arr, left, mid, right, tmp);
}

// MergeSort wrappers
void merge_sort_improved(std::vector<uint32_t>& arr) {
    max_recursion_depth = 0;
    if(arr.size() < 2) {
        return;
    }
    std::vector<uint32_t> tmp(arr.size());
    mergesort_improved_recursion(arr, 0, arr.size() - 1, tmp, 1);
}

void merge_sort_naive(std::vector<uint32_t>& arr) {
    max_recursion_depth = 0;
    if(arr.size() < 2) {
        return;
    }
    mergesort_naive_recursion(arr, 0, arr.size() - 1, 1);
}


// HeapSort  

void heap_siftdown_iterative(std::vector<uint32_t>& arr, size_t root, size_t end) {
    while(true) {
        size_t child = root * 2 + 1;
        if(child > end) {
            break;
        }
        if(child + 1 <= end && arr[child] < arr[child + 1]) {
            ++child;
        }
        if(arr[root] < arr[child]) {
            std::swap(arr[root], arr[child]);
            root = child;
        } else {
            break;
        }
    }
}


void heap_siftdown_recursive(std::vector<uint32_t>& arr, size_t root, size_t end, size_t depth) {
    if(depth > max_recursion_depth) {
        max_recursion_depth = depth;
    }
    size_t child = root * 2 + 1;
    if(child > end) {
        return;
    }
    if(child + 1 <= end && arr[child] < arr[child + 1]) {
        ++child;
    }
    if(arr[root] < arr[child]) {
        std::swap(arr[root], arr[child]);
        heap_siftdown_recursive(arr, child, end, depth + 1);
    }
}

// HeapSort wrappers
void heap_sort_iterative(std::vector<uint32_t>& arr) {
    max_recursion_depth = 0;
    const size_t size = arr.size();
    if(size < 2) {
        return;
    }
    for(size_t start{size / 2}; start > 0; --start) {
        heap_siftdown_iterative(arr, start - 1, size - 1);
    }
    for(size_t end{size - 1}; end > 0; --end) {
        std::swap(arr[0], arr[end]);
        heap_siftdown_iterative(arr, 0, end - 1);
    }
}

void heap_sort_recursive(std::vector<uint32_t>& arr) {
    max_recursion_depth = 0;
    const size_t size = arr.size();
    if (size < 2) {
        return;
    }
    for(size_t start{size / 2}; start > 0; --start) {
        heap_siftdown_recursive(arr, start - 1, size - 1, 1);
    }
    for(size_t end{size - 1}; end > 0; --end) {
        std::swap(arr[0], arr[end]);
        heap_siftdown_recursive(arr, 0, end - 1, 1);
    }
}

// CountingSort

void counting_sort(std::vector<uint32_t>& arr) {
    max_recursion_depth = 0;
    if(arr.size() < 2) {
        return;
    }
    const uint32_t max_value = *std::max_element(arr.begin(), arr.end());
    std::vector<size_t> count(static_cast<size_t>(max_value) + 1, 0);
    for(size_t j{0}; j < arr.size(); ++j) {
        const uint32_t x = arr[j];
        ++count[static_cast<size_t>(x)];
    }
    size_t write_index = 0;
    for(size_t i{0}; i <= max_value; ++i) {
        const size_t c = count[static_cast<size_t>(i)];
        for(size_t j{0}; j < c; ++j) {
            arr[write_index] = static_cast<uint32_t>(i);
            ++write_index;
        }
    }
}

// RadixSort

void radix_sort(std::vector<uint32_t>& arr) {
    max_recursion_depth = 0;
    if(arr.size() < 2) {
        return;
    }
    std::vector<uint32_t> output(arr.size());
    for(int shift{0}; shift < 32; shift += 8) {
        uint32_t count[256] = {};
        for(size_t i{0}; i < arr.size(); ++i) {
            const uint32_t x = arr[i];
            const size_t byte = static_cast<size_t>((x >> shift) & 255U);
            ++count[byte];
        }
        uint32_t pos[256];
        pos[0] = 0;
        for(int i{1}; i < 256; ++i) {
            pos[i] = pos[i - 1] + count[i - 1];
        }
        for(size_t j{0}; j < arr.size(); ++j) {
            const uint32_t x = arr[j]; 
            const size_t byte = static_cast<size_t>((x >> shift) & 255U);
            output[pos[byte]] = x;
            ++pos[byte];
        }
        arr.swap(output);
    }
}

// BucketSort

void bucket_sort(std::vector<uint32_t>& arr) {
    max_recursion_depth = 0;
    const size_t n = arr.size();
    if(n < 2) {
        return;
    }
    const uint32_t min_value = *std::min_element(arr.begin(), arr.end());
    const uint32_t max_value = *std::max_element(arr.begin(), arr.end());
    if(min_value == max_value) {
        return;
    }
    const size_t range = static_cast<size_t>(max_value) - min_value;
    const size_t range_count = range + 1;
    const size_t max_buckets = 65536;
    size_t bucket_count = std::min(n, max_buckets);
    if(range_count < bucket_count) {
        bucket_count = static_cast<size_t>(range_count);
    }
    if(bucket_count == 0) {
        bucket_count = 1;
    }
    std::vector<std::vector<uint32_t>> buckets(bucket_count);
    for(size_t i{0}; i < arr.size(); ++i) {
        const uint32_t x = arr[i];
        size_t index = 0;
        if(bucket_count > 1) {
            index = static_cast<size_t>( (static_cast<size_t>(x) - min_value) *(bucket_count - 1) / range
            );
        }
        buckets[index].push_back(x);
    }
    size_t write_index = 0;
    for(size_t i{0}; i < buckets.size(); ++i) {
        std::vector<uint32_t>& bucket = buckets[i];
        insertion_sort(bucket);
        for(size_t i{0}; i < bucket.size(); ++i) {
            const uint32_t x = bucket[i];
            arr[write_index] = x;
            ++write_index;
        }
    }
}

// Built-in sort

void builtin_sort(std::vector<uint32_t>& arr) {
    max_recursion_depth = 0;
    std::sort(arr.begin(), arr.end());
}