#include <iostream>
#include <random>


void random_number(int64_t&, int64_t, int64_t);
void input(int64_t&, int64_t = {}, int64_t = {});
void output(int64_t&, int64_t = {}, int64_t = {});
void array_i_o(int64_t*, size_t, void(*)(int64_t&, int64_t, int64_t), int64_t = {}, int64_t = {});
int64_t* merge_sort(int64_t*, size_t, int64_t*, size_t);


int main() {
    srand(time(0));
    size_t array_size{};
    int64_t random_array_input_upper_border{}, random_array_input_bottom_border{};
    std::cin >> array_size >> random_array_input_bottom_border >> random_array_input_upper_border;
    int64_t* array = new int64_t[array_size];
    array_i_o(array, array_size, random_number, random_array_input_bottom_border, random_array_input_upper_border);
    array_i_o(array, array_size, output);
    std::cout << "\n\n\n";
    array_i_o(merge_sort(array, array_size / 2, array + (array_size / 2), array_size - array_size / 2), array_size, output);

    delete[] array;

    return 0;
}


void random_number(int64_t& A_i, int64_t bottom_border, int64_t upper_border) {
    A_i = rand() % (upper_border - bottom_border + 1) + bottom_border;
}

void input(int64_t& A_i, int64_t a, int64_t b) {
    std::cin >> A_i;
}

void output(int64_t& A_i, int64_t a, int64_t b) {
    std::cout << A_i << ' ';
}

void array_i_o(int64_t* array, size_t array_size, void(*operation)(int64_t&, int64_t, int64_t), int64_t bottom_border, int64_t upper_border) {
    for(size_t i{0}; i < array_size; ++i) {
        operation(array[i], bottom_border, upper_border);
    }
}

int64_t* merge_sort(int64_t* array1, size_t array1_size, int64_t* array2, size_t array2_size) {
    if(array1_size != 1) {
        array1 = merge_sort(array1, array1_size / 2, array1 + (array1_size / 2), array1_size - array1_size / 2);
    }
    if(array2_size != 1) {
        array2 = merge_sort(array2, array2_size / 2, array2 + (array2_size / 2), array2_size - array2_size / 2);
    }
    size_t size = array1_size + array2_size;
    int64_t* new_array = new int64_t[size];
    for(size_t i{0}, j{0}, k{0}; i < size; ++i) {
        if(array1[j] < array2[k]) {
            new_array[i] = array1[j];
            ++j;
        }
        else {
            new_array[i] = array2[k];
            ++k;
        }
        if(j == array1_size) {
            for(size_t I{i + 1}; I < size; ++I) {
                new_array[I] = array2[k];
                ++k;
            }
            break;
        }
        if(k == array2_size) {
            for(size_t I{i + 1}; I < size; ++I) {
                new_array[I] = array1[j];
                ++j;
            }
            break;
        }
    
    }

    return new_array;
}