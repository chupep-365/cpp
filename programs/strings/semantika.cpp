#include <iostream>
#include <string>

struct __wrd                                                           // a struct to store a word,
{                                                                      // its size and position in a string
    std::string wrd;                                                   // together
    size_t wrd_size;
    size_t wrd_index;
};


bool is_palindrom(std::string);
u_int32_t plndrms_amount(std::string, std::string);
__wrd* plndrm_arr(std::string, std::string);
std::string toupper(std::string);
__wrd* top_longest(__wrd*, u_int32_t, u_int32_t);
void sort_lexicographic(__wrd*&, u_int32_t);
void sort_index(__wrd*&, u_int32_t);
void semantic(__wrd*&, __wrd*&, u_int32_t, std::string&);


int main() {
    std::string str = {"The tactical radar detected"
                       " a fast racecar while the civic"
                       " leader used a kayak to cross the river" }; 
    u_int32_t k{3};                                                                  // top k longest palindromes to replace
    std::string delim = {" ,.!?;:"};
    u_int32_t plndrms = plndrms_amount(str, delim);
    if(plndrms < k) {
        k = plndrms;
    }
    __wrd* all_plndrms_arr = plndrm_arr(str, delim);                                // create array of all palindromes
    __wrd* longest_plndrms_arr = top_longest(all_plndrms_arr, plndrms, k);          // then take top k longest of them
    sort_index(longest_plndrms_arr, k);                                             // and sort them by index
    __wrd* plndrms_top = new __wrd[k];                                              // create array containing palindromes
    for(size_t i{0}; i < k; ++i) {                                                  // |
        plndrms_top[i].wrd = longest_plndrms_arr[i].wrd;                            // |
        plndrms_top[i].wrd_index = longest_plndrms_arr[i].wrd_index;                // |
        plndrms_top[i].wrd_size = longest_plndrms_arr[i].wrd_size;                  // |
    }                                                                               // |
    sort_lexicographic(plndrms_top, k);                                             // in the order we need
    semantic(longest_plndrms_arr, plndrms_top, k, str);
    std::cout << str << '\n';

    delete[] all_plndrms_arr;
    delete[] longest_plndrms_arr;
    delete[] plndrms_top;
    return 0;
}

bool is_palindrom(std::string str) {                                    // check if the word is palindrome or not
    size_t str_size = str.size();                                     
    if(str_size == 1) {
        return false;
    }
   std::string::iterator L = str.begin();
   std::string::iterator R = str.end() - 1; 
   while(L < R) {
        if(*L != *R) {
            return false;
        }
        ++L;
        --R;
   }
    return true;
}

u_int32_t plndrms_amount(std::string str, std::string delim) {         // count amount of palindromes in a string
    u_int32_t plndrm_counter{0};
    std::string temp_wrd{""};
    for(size_t wrd_end{0}, wrd_start = str.find_first_not_of(delim, wrd_end); wrd_start != std::string::npos; wrd_start = str.find_first_not_of(delim, wrd_end)) {
        wrd_end = str.find_first_of(delim, wrd_start);
        temp_wrd = str.substr(wrd_start, wrd_end - wrd_start);
        if(is_palindrom(toupper(temp_wrd))) {
            ++plndrm_counter;
        }
        temp_wrd.clear();
    }
    return plndrm_counter;
}

__wrd* plndrm_arr(std::string str, std::string delim) {                 // create an array of palindromes
    u_int32_t plndrms = plndrms_amount(str, delim);
    std::string temp_wrd{""};
    __wrd* plndrm_arr = new __wrd[plndrms];
    for(size_t i{0} ,wrd_end{0}, wrd_start = str.find_first_not_of(delim, wrd_end); wrd_start != std::string::npos; wrd_start = str.find_first_not_of(delim, wrd_end)) {
        wrd_end = str.find_first_of(delim, wrd_start);
        temp_wrd = str.substr(wrd_start, wrd_end - wrd_start);
        if(is_palindrom(toupper(temp_wrd))) {
            plndrm_arr[i].wrd = temp_wrd;
            plndrm_arr[i].wrd_size = wrd_end - wrd_start;
            plndrm_arr[i].wrd_index = wrd_start;
            ++i;
        }
        temp_wrd.clear();
    }
    return plndrm_arr;
}

std::string toupper(std::string str) {                                 // transform a word to uppercase for 
    size_t str_size = str.size();                                      // proper checking and comparison
    for(size_t i{0}; i < str_size; ++i) {
        if(str[i] > 96 && str[i] < 123) {
            str[i] ^= 0x20;
        }
    }
    return str;
}

__wrd* top_longest(__wrd* arr, u_int32_t arr_size, u_int32_t k) {       // create array of k longest palindromes 
    u_int32_t arr_max_index{};                                          
    for(size_t i{0}; i < arr_size - 1; ++i) {                           
        arr_max_index = i;
        for(size_t j{i + 1}; j < arr_size; ++j) {
            if(arr[j].wrd_size > arr[arr_max_index].wrd_size) {         // used simple sorting algorithms
                arr_max_index = j;                                      // cuz im a bit lazy
            } 
        }
        if(arr_max_index != i) {
            std::swap(arr[i], arr[arr_max_index]);
        }
    }
    __wrd* top_arr = new __wrd[k];
    for(size_t i{0}; i < k; ++i) {
        top_arr[i] = arr[i];
    }
    
    return top_arr;
}

void sort_lexicographic(__wrd*& arr, u_int32_t arr_size) {              // sort array in lexicographical order
    for(size_t i{1}; i < arr_size; ++i) {                               // of words
        size_t j = i;
        while(j > 0 && toupper(arr[j].wrd).compare(toupper(arr[j - 1].wrd)) < 0) {
            std::swap(arr[j], arr[j - 1]);
            --j;
        }
    }
}

void sort_index(__wrd*& arr, u_int32_t arr_size) {                      // sort array in ascending order of
    for(size_t i{1}; i < arr_size; ++i) {                               // indices of palindromes in the string
        size_t j = i;
        while(j > 0 && arr[j].wrd_index < arr[j - 1].wrd_index) {
            std::swap(arr[j], arr[j - 1]);
            --j;
        }
    }
}

void semantic(__wrd*& plndrms_arr, __wrd*& plndrms_top, u_int32_t k, std::string& str) {        // change the order of the longest
    int64_t padding{0};                                                                         // palindromes in the string to 
    for(size_t i{0}; i < k; ++i) {                                                              // lexicographical
        padding = -(plndrms_arr[i].wrd_size - plndrms_top[i].wrd_size);
        str.erase(plndrms_arr[i].wrd_index, plndrms_arr[i].wrd_size);                           // we simply cut the i-th longest palindrome from the string and  
        str.insert(plndrms_arr[i].wrd_index, plndrms_top[i].wrd);                               // insert in its place the i-th palindrome from the lexicographically correct array
        for(size_t j{i + 1}; j < k; ++j) {
            if(plndrms_arr[j].wrd_index < plndrms_top[i].wrd_index) {                           
                plndrms_arr[j].wrd_index += padding;                                            // shifting 
            }
        }
    }
}