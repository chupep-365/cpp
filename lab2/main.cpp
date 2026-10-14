#include "maestro.h"

int main() {
    std::cout << "enter name of input file: ";
    std::string filename{};
    std::cin >> filename;
    std::ifstream fin = create_ifstream(filename);
    std::vector<std::string> words = read_file(fin);
    fin.close();
    swap_min_and_max(words);
    std::ofstream fout(filename, std::ios::out);
    write_file(fout, words);
    fout.close();

    return 0;
}

/*  aaa bbb ccc DDDDDDDDDDDDD
    eee f ggg hhh kkk lll mmm nnn ooo ppp qqq rrr sss ttt
    www uuu vvv jjj xxx yyy zzz 
*/