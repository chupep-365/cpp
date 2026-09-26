#include <iostream>
#include <fstream>
#include <cmath>

bool check_input_file(std::ifstream&);
u_int16_t check_amount(std::ifstream&);
void sqmtrxalloc(int64_t**&, const size_t);
void sqmtrxfill(int64_t**&, const size_t, std::ifstream&);

int main() {
    std::ofstream fout("input.txt", std::ios::app);
    try {
        std::ifstream fin1("input.txt", std::ios::in);
        check_input_file(fin1);
        size_t size = (size_t)ceil(sqrt(check_amount(fin1)));
        int64_t** mtrx = nullptr;
        sqmtrxalloc(mtrx, size);
        std::ifstream fin2("input.txt", std::ios::in);
        check_input_file(fin2);
        sqmtrxfill(mtrx, size, fin2);
    }
    catch(const char* msg) {
        std::cout << msg;
        return 1;
    }
}

bool check_input_file(std::ifstream& fin) {
    if(!fin.good()) {
        throw "file isnt exist\n";
    }
    if(!fin) {
        throw "input file error\n";
    }
    if(fin.peek() == EOF) {
        throw "file is empty\n";
    }
    return true;
}

u_int16_t check_amount(std::ifstream& fin) {
    u_int16_t counter{};
    int64_t elmnt{};
    while(fin >> elmnt) {
        counter++;
    }
    return counter;
}

void sqmtrxalloc(int64_t**& mtrx, const size_t size) {
    mtrx = new int64_t* [size];
    for(size_t i{0}; i < size; ++i) {
        mtrx[i] = new int64_t[size];
    }
    for(size_t i{}; i < size; ++i) {
        for(size_t j{0}; j < size; ++j) {
            mtrx[i][j] = 0;
        }
    }
}

void sqmtrxfill(int64_t**& mtrx, const size_t size, std::ifstream& fin) {
    int64_t elmnt{};
    size_t i{0};
    size_t j{0};
    while(fin >> elmnt) {
        mtrx[i][j] = elmnt;
        j++;
        if(!(j < size)) {
            j = 0;
            i++;
        }
        if(!(i < size)) {
            break;
        }
    }
}