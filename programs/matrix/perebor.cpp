#include <iostream>

int main() {
    int64_t a[12];
    int64_t n = 10;
    for(size_t j{0}; j < 12; ++j)
        a[j] = 0;
    while(a[0] == 0) {
        for(size_t i{1}; i <= n; ++i){
            std::cout << a[i];
        }
        std::cout << '\n';
        size_t i = {n};
        while(a[i] == n - 1) {
            a[i] = 0;
            --i;
        }
        ++a[i];
    }
}