#include "maestro.h"

int main() {
    std::cout << "enter name of input file: ";
    std::string filename{};
    std::cin >> filename;
    try
    {
        std::ifstream fin = create_ifstream(filename);
        std::string text = read_file(fin);
        fin.close();
        swap_min_and_max(text);
        std::ofstream fout(filename, std::ios::out);
        write_file(fout, text);
        fout.close();
    }
    catch(const char* err)
    {
        std::cerr << err;
        return -1;
    }

    return 0;
}

/*  
    
    aaa bbb ccc DDDDDDDDDDDDD
    eee f ggg hhh kkk lll mmm nnn ooo ppp qqq rrr sss ttt
    www uuu vvv jjj xxx yyy zzz 
    .,..  
*/