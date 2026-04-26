#define _CRT_SEQURE_NO_WARNINGS
#include <iostream>
#include <cstring>

u_int16_t wrds_count(const char*, const char*);
char** tokenize(char*, const char*);
bool is_anogram(const char*, const char*);
char*** anogramize(char**, u_int16_t, u_int16_t&);


int main() {
    char* string = new char[1000];
    const char* delim{" ,.:;!?"};
    std::cin.getline(string, 1000);
    u_int16_t wrds = wrds_count(string, delim);
    char** token_arr = tokenize(string, delim);
    for(size_t i{0}; i < wrds; ++i) {
        std::cout << token_arr[i] << ' ';
    }
    std::cout << '\n' << wrds << '\n';
    u_int16_t anogram_grps{1};
    char*** anogram_arr = anogramize(token_arr, wrds, anogram_grps);
    for(size_t i{0}; i < anogram_grps; ++i) {
        std::cout << i + 1 << ") ";
        while(*anogram_arr[i] != (char*)"\0") {
            std::cout << *(anogram_arr[i]) << ' ';
            ++(anogram_arr[i]);
        }
        std::cout << '\n';
    }
}

u_int16_t wrds_count(const char* str, const char* delim) {
    u_int16_t counter{0};
    str += strspn(str, delim);
    while(*str != '\0') {
        str += strspn(str, delim);
        ++counter;
        str += strcspn(str, delim);
    }
    return counter;
}

char** tokenize(char* str, const char* delim) {
    u_int16_t wrds = wrds_count(str, delim);
    char** token_arr = new char*[wrds];
    char* token = strtok(str, delim);
    while(token != NULL) {
        *token_arr = token;
        token = strtok(NULL, delim);
        ++token_arr;
    }
    token_arr -= wrds;
    return token_arr;
}

bool is_anogram(const char* wrd_A, const char* wrd_B) {
    if(strlen(wrd_A) != strlen(wrd_B)) {
        return false;
    }
    char* wrd_temp = new char[strlen(wrd_B)];
    strcpy(wrd_temp, wrd_B);
    while(*wrd_A != '\0') {
        if(strchr(wrd_temp, *wrd_A) == NULL) {
            return false;
        }
        *(strchr(wrd_temp, *wrd_A)) = ' ';
        ++wrd_A;
    }
    return true;
}

char*** anogramize(char** token_arr, u_int16_t wrds, u_int16_t& ngrm_grp_nmbr) {
    u_int16_t* anogram_group_arr = new u_int16_t[wrds];
    for(u_int16_t i{0}; i < wrds; ++i) {
        anogram_group_arr[i] = 0;
    }
    ngrm_grp_nmbr = {1};
    for(u_int16_t i{0}; i < wrds; ++i) {
        if(anogram_group_arr[i] != 0) {
            continue;
        }
        anogram_group_arr[i] = ngrm_grp_nmbr;
        for(u_int16_t j = i+1; j < wrds; ++j) {
            if(anogram_group_arr[j] == 0 && is_anogram(token_arr[i], token_arr[j])) {
                anogram_group_arr[j] = ngrm_grp_nmbr;
            }
        }
        ++ngrm_grp_nmbr;
    }
    --ngrm_grp_nmbr;
    u_int16_t* grps_size_arr = new u_int16_t[ngrm_grp_nmbr];
    for(u_int16_t i{0}; i < ngrm_grp_nmbr; ++i) {
        grps_size_arr[i] = 0;
    }
    for(u_int16_t i{0}; i < wrds; ++i) {
        ++grps_size_arr[anogram_group_arr[i] - 1];
    }
    char*** anograms_arr = new char**[ngrm_grp_nmbr];
    for(u_int16_t i{0}; i < ngrm_grp_nmbr; ++i) {
        anograms_arr[i] = new char* [grps_size_arr[i] + 1];
    }
    for(u_int16_t i{0}; i < wrds; ++i) {
        *(anograms_arr[anogram_group_arr[i] - 1]) = token_arr[i];
        ++(anograms_arr[anogram_group_arr[i] - 1]);
        *(anograms_arr[anogram_group_arr[i] - 1]) = (char*)"\0";
    }
    for(u_int16_t i{0}; i < ngrm_grp_nmbr; ++i) {
        anograms_arr[i] -= grps_size_arr[i];
    }
    delete[] grps_size_arr;
    delete[] anogram_group_arr;

    return anograms_arr;
}