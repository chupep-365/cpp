#include "task_func.h"

void swap_min_and_max(std::vector<std::string>& arr) {
    size_t max_ind{0};
    size_t min_ind{0};
    for(size_t i{}; i < arr.size(); ++i) {
        if(arr[i].size() > arr[max_ind].size()) {
            max_ind = i;
            continue;
        }
        if(arr[i].size() < arr[min_ind].size()) {
            min_ind = i;
            continue;
        }
    }
    if(max_ind != min_ind) {
        std::swap(arr[max_ind], arr[min_ind]);
    }
}