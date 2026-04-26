#include <iostream>
#include <cstring>

char** mstrtok(char*, const char*);
u_int64_t wrds_count(const char*, const char*);


int main() {
    char* string = new char[1000];
    const char* delim_str{" ,.:!?\n"};
    std::cin.getline(string, 1000);
    u_int64_t wrds = wrds_count(string, delim_str);
    std::cout << wrds << '\n';
    char** token_arr = mstrtok(string, delim_str);
    for(size_t i{0}; i < wrds; ++i) {
        std::cout << token_arr[i] << '\n';
    }
}

u_int64_t wrds_count(const char* str, const char* delim) {
    u_int64_t counter{0};
    bool flag{false};
    while(*str != '\0') {
        if(strpbrk(str, delim) != str) {
            flag = true;
            ++str;
            continue;
        }
        if(flag == true) {                 
            flag = false;
            ++counter;
            ++str;
            continue;
        }
        ++str;
    }
    if(flag == true) {
        ++counter;
    }
    return counter;
}

char** mstrtok(char* str, const char* delim) {
    char* token = str;
    u_int64_t wrds = wrds_count(str, delim);
    char** token_arr = new char* [wrds];
    char** tkn_arr_ptr = token_arr;
    while(wrds != 0) {
        while(strpbrk(str, delim) == str) {
            ++str;
            token = str;
        }
        while(strpbrk(str, delim) != str && *str != '\0') {
            ++str;
        }
        *str = '\0';
        *token_arr = token;
        ++token_arr;
        ++str;
        token = str;
        --wrds;
    }
    return tkn_arr_ptr;
}