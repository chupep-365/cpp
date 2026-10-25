#ifndef FILE_WORK_H
#define FILE_WORK_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>


bool check_input_file(std::ifstream&);
std::ifstream create_ifstream(const std::string);
std::string read_file(std::ifstream&);
void write_file(std::ofstream&, const std::string&);


#endif