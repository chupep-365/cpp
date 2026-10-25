#ifndef TASK_FUNC_H
#define TASK_FUNC_H

#include <vector>
#include <string>


struct Word {
    size_t start;
    size_t length;
};

bool is_not_delim(char);
void swap_min_and_max(std::string&);

#endif