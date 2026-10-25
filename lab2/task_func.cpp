#include "task_func.h"

bool is_not_delim(char ch) {
    unsigned char symbol = (unsigned char)(ch);
    return std::isalnum(symbol) != 0;
}

void swap_min_and_max(std::string& text) {
    std::vector<Word> words;
    for(size_t i{0}; i < text.size(); ++i) {
        if(!is_not_delim(text[i])) {
            continue;
        }
        size_t start = i;
        while(i < text.size() && is_not_delim(text[i])) {
            ++i;
        }
        words.push_back({start, i - start});
    }
    if(words.size() < 2) {
        return;
    }
    size_t shortest_index = 0;
    size_t longest_index = 0;
    for(size_t i{1}; i < words.size(); ++i) {
        if(words[i].length < words[shortest_index].length) {
            shortest_index = i;
        }
        if(words[i].length > words[longest_index].length) {
            longest_index = i;
        }
    }
    if(shortest_index == longest_index) {
        return;
    }
    Word shortest = words[shortest_index];
    Word longest = words[longest_index];
    std::string shortest_word =
        text.substr(shortest.start, shortest.length);
    std::string longest_word =
        text.substr(longest.start, longest.length);
    std::string result;
    for(size_t i{0}; i < text.size();) {
        if(i == shortest.start) {
            result += longest_word;
            i += shortest.length;
            continue;
        } 
        if(i == longest.start) {
            result += shortest_word;
            i += longest.length;
            continue;
        } 
        result += text[i];
        ++i;
    }
    text = result;
}