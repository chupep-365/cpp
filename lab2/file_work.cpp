#include "file_work.h"

bool check_input_file(std::ifstream& fin) {
    if(!fin.good()) {
        throw "\nFile doesnt exist!\n";
    }
    if(!fin) {
        throw "\nCant open file!\n";
    }
    if(fin.peek() == EOF) {
        throw "\nFile is empty!\n";
    }
    return true;
}

std::ifstream create_ifstream(const std::string filename) {
    std::cout << "Opening file: " << filename << "...\n";
    std::ifstream fin(filename, std::ios::in);
    check_input_file(fin);
    std::cout << "File opened!\n";
    return fin;
}

std::string read_file(std::ifstream& fin) {
    std::cout << "Reading file...\n";
    std::string text;
    char ch;
    while(fin.get(ch)) {
        text += ch;
    }
    std::cout << "File read!\n";
    return text;
}

void write_file(std::ofstream& fout, const std::string& text) {
    fout << text;
    fout.close();
    std::cout << "\nDone!\n";
}