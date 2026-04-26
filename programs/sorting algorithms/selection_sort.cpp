#include <iostream>


void array_i_o(int64_t*, size_t, void(*operation)(int64_t& A_i));
void input(int64_t&);
void output(int64_t&);
void array_sort(int64_t*, size_t);
void swap(int64_t&, int64_t&);


int main() {
    size_t array_size{};
    std::cin >> array_size;
    int64_t* array = new int64_t[array_size];
    array_i_o(array, array_size, input);
    array_sort(array, array_size);
    array_i_o(array, array_size, output);

    delete array;
    
    return 0;
}


void array_i_o(int64_t* array, size_t array_size, void(*operation)(int64_t& A_i)) {
    for(size_t i{0}; i < array_size; ++i) {
        operation(array[i]);
    }
}

void input(int64_t& A_i) {
    std::cin >> A_i;
}

void output(int64_t& A_i) {
    std::cout << A_i << ' ';
}

void array_sort(int64_t* array, size_t array_size) {
    u_int64_t array_min_index{};
    for(size_t i{0}; i < array_size - 1; ++i) {
        array_min_index = {i};
        for(size_t j{i + 1}; j < array_size; ++j) {
            if(array[j] < array[array_min_index]) {
                array_min_index = j;
            } 
        }
        if(array_min_index != i) {
            swap(array[i], array[array_min_index]);
        }
    }
}

void swap(int64_t& a, int64_t& b) {
    a ^= b;
    b ^= a;
    a ^= b;
}