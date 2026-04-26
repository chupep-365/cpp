#include <iostream>

void compute_primes(bool*, u_int32_t);

int main()
{
    u_int32_t number{};
    std::cin >> number;
    bool* sieve = new bool[number];
    compute_primes(sieve, number);
}

void compute_primes(bool* sieve, u_int32_t number) {
    for(u_int32_t i{2}; i <= number; ++i) {
        sieve[i] = true;
    }
    u_int32_t sieve_iterator{};
    u_int32_t num_iterator{2};
    while(num_iterator * num_iterator <= number) {
        if(sieve[num_iterator]) {
            sieve_iterator = num_iterator * num_iterator;
            while(sieve_iterator <= number) {
                sieve[sieve_iterator] = false;
                sieve_iterator += num_iterator;
            }
        }
        ++num_iterator;
    }
    for(u_int32_t i{1}; i <= number; ++i) {
        if(sieve[i]) {
            std::cout << i << ' ';
        }
    } 
    delete sieve;
}