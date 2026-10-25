#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

void is_open(std::ifstream&);
void create_bin(std::ifstream&);
void read_bin(std::ifstream&);

struct Student
{
    char surname[20];
    int16_t marks[3];
};


int main() {
    try
    {
        std::ifstream fin("text.txt");
        is_open(fin);
        create_bin(fin);
        std::ifstream bin("out.bin", std::ios::binary);
        read_bin(bin);
    }
    catch(const char* code)
    {
        std::cerr << code;
    }
    

    return 0;
}

void is_open(std::ifstream& fin) {
    if(!fin) {
        throw "\nError!\n";
    }
}

void create_bin(std::ifstream& fin) {
    Student person;
    std::string delims{};
    std::getline(fin, delims);
    std::string temp{};
    std::string name{};
    size_t beg{}, end{};
    std::ofstream bin_out("out.bin", std::ios::binary);
    while(std::getline(fin, temp)) {
        if(temp.empty()) {
            continue;
        }
        beg = temp.find_first_not_of(delims);
        if(beg == std::string::npos) {
            continue;
        }
        end = temp.find_first_of(delims, beg);
        if(end == std::string::npos) {
            end = temp.length();
        }
        name = temp.substr(beg, end - beg);
        strcpy(person.surname, name.c_str());
        for(size_t i{}; i < 3; ++i) {
            beg = temp.find_first_not_of(delims, end);
            end = temp.find_first_of(delims, beg);
            person.marks[i] = stoi(temp.substr(beg, end - beg));
        }
        bin_out.write((char*)&person, sizeof(person));
    }
    bin_out.close();
    fin.close();
}

void read_bin(std::ifstream& fin) {
    Student person;
    while(fin.read(reinterpret_cast<char*>(&person), sizeof(person))) {
        std::cout << person.surname << ": " << person.marks[0] << ' ' << person.marks[1] 
                    << ' ' << person.marks[2] << '\n'; 
    }
}