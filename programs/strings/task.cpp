#include <iostream>
#include <cstring>

size_t string_length(char*);
void display_string_length(char*);
void display_doubled_string(char*);

int main() {
     char* string = new char[67];
    std::cout << "Enter your string:\n";
    std::cin.getline(string, 10000);
    std::cout << "You entered:\n" << string << '\n';
    display_string_length(string);
    display_doubled_string(string);
    
}


size_t string_length(char* string) {
    return strlen(string);
}

void display_string_length(char* string) {
    std::cout << "String length is: " << string_length(string) << '\n';
}

void display_doubled_string(char* string) {
    char* string2 = new char[2 * string_length(string) + 1];
    strcpy(string2, string);
    strcat(string2, string);
    std::cout << "Your string, doubled: " << string2 << '\n';
    delete[] string2;
}