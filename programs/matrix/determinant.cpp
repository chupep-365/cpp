#include <iostream>
#include <iomanip>
#include <random>

int64_t** create_sqr_mtrx(uint32_t);
void sqr_mtrx_random_fill(int64_t**&, uint32_t, int64_t, int64_t);
void sqr_mtrx_filling(int64_t**&, uint32_t);
void sqr_mtrx_manual_input(int64_t**&, uint32_t);
int64_t random_int(int64_t, int64_t);
void mtrx_output(int64_t**, uint32_t);




int main() {
    srand(time(NULL));
    uint32_t matrix_size{};
    std::cout << "Enter size of square matrix: ";
    std::cin >> matrix_size;
    int64_t** matrix = create_sqr_mtrx(matrix_size);
    sqr_mtrx_filling(matrix, matrix_size);
    
}


int64_t** create_sqr_mtrx(uint32_t matrix_size) {
    int64_t** matrix = new int64_t* [matrix_size];
    for(size_t i{0}; i < matrix_size; ++i) {
        matrix[i] = new int64_t[matrix_size];
    }
    return matrix;
}

void mtrx_output(int64_t** matrix, uint32_t matrix_size) {
    for(size_t i{0}; i < matrix_size; ++i) {
        for(size_t j{0}; j < matrix_size; j++) {
            std::cout << std::setw(4) << matrix[i][j];
        }
        std::cout << '\n';
    }
}

int64_t random_int(int64_t lower_border, int64_t upper_border) {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution < int64_t > dist(lower_border, upper_border);
    return dist(rng);
}

void sqr_mtrx_random_fill(int64_t**& matrix, uint32_t matrix_size, int64_t lower_border, int64_t upper_border) {
    for(size_t i{0}; i < matrix_size; ++i) {
        for(size_t j{0}; j < matrix_size; j++) {
            matrix[i][j] = random_int(lower_border, upper_border);
        }
    }
}

void sqr_mtrx_manual_input(int64_t**& matrix, uint32_t matrix_size) {
    for(size_t i{0}; i < matrix_size; ++i) {
        for(size_t j{0}; j < matrix_size; j++) {
            std::cin >> matrix[i][j];
        }
    }
}

void sqr_mtrx_filling(int64_t**& matrix, uint32_t matrix_size) {
    std::cout << "Select matrix filling:\n1) Manual\n2) Random\n";
    uint8_t a{};
    std::cin >> a;
    switch (a)
    {
    case '1': {
        std::cout << "Enter your matrix:\n" << std::setfill('*') << std::setw(matrix_size);
        sqr_mtrx_manual_input(matrix, matrix_size);
        std::cout << std::setfill('*') << std::setw(matrix_size);
        break;
    }
    case '2': {
        std::cout << "Enter minimum and maximum of matrix`s element range: ";
        int64_t lower_border{};
        int64_t upper_border{};
        std::cin >> lower_border >> upper_border;
        sqr_mtrx_random_fill(matrix, matrix_size, lower_border, upper_border);
        mtrx_output(matrix, matrix_size);
        break;
    }
    default: {
        std::cout << "ERROR!\n";
        break;
    }
    }
}