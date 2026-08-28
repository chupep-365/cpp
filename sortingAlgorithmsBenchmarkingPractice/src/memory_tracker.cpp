#include "memory_tracker.h"
#include <new>
#include <cstdlib>
#include <atomic>

static std::atomic<size_t> current_heap{0};
static std::atomic<size_t> peak_heap{0};

void reset_memory_tracker() {
    current_heap = 0;
    peak_heap = 0;
}

size_t get_peak_memory_bytes() {
    return peak_heap.load();
}

// Перегрузка new
void* operator new(size_t size) {
    void* raw = malloc(size + sizeof(size_t));
    if (!raw) throw std::bad_alloc();
    *static_cast<size_t*>(raw) = size;
    void* ptr = static_cast<char*>(raw) + sizeof(size_t);
    current_heap += size;
    size_t current = current_heap.load();
    size_t peak = peak_heap.load();
    while (current > peak && !peak_heap.compare_exchange_weak(peak, current)) {}
    return ptr;
}

// Перегрузка delete
void operator delete(void* ptr) noexcept {
    if (!ptr) return;
    void* raw = static_cast<char*>(ptr) - sizeof(size_t);
    size_t size = *static_cast<size_t*>(raw);
    current_heap -= size;
    free(raw);
}

// Перегрузки для массивов
void* operator new[](size_t size) { return operator new(size); }
void operator delete[](void* ptr) noexcept { operator delete(ptr); }

// Перегрузки C++14 с указанием размера (sized deallocation)
void operator delete(void* ptr, size_t size) noexcept {
    if (!ptr) return;
    current_heap -= size;
    void* raw = static_cast<char*>(ptr) - sizeof(size_t);
    free(raw);
}
void operator delete[](void* ptr, size_t size) noexcept {
    operator delete(ptr, size);
}