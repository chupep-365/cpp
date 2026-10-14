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
    try
    {
        check_input_file(fin);
    }
    catch(const char* err)
    {
        std::cerr << err;
        return {};
    }
    std::cout << "File opened!\n";
    return fin;
}

std::vector<std::string> read_file(std::ifstream& fin) {
    std::vector<std::string> words{};
    std::string temp{};
    while(fin >> temp) {
        words.push_back(temp);
    }
    return words;
}

void write_file(std::ofstream& fout, const std::vector<std::string>& words) {
    for(size_t i{}; i < words.size(); ++i) {
        fout << words[i];
    }
    std::cout << "\nDone!\n";
}