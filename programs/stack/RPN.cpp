#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

#define TNT template <class T>

TNT 
struct Stack
{
    T* st{nullptr};
    size_t size{};
    size_t top{};
    float resize_factor{};
};

TNT
void init(Stack<T>&, const size_t&, const float&);

TNT
void clear(Stack<T>&);

TNT
bool is_empty(const Stack<T>&);

TNT
void push(Stack<T>&, const T&);

TNT
T pop(Stack<T>&);

TNT
T top(const Stack<T>&); 
size_t tkns_mnt(const std::string&);
std::string* tokenize(const std::string&);
std::string RPN(const std::string&);
int16_t alpha_prior(const std::string&);
double RPN_calculate(const std::string&);

TNT 
void resize(Stack<T>&, const size_t&);


int main() {
    //std::string string{"25 + 17 * ( 67 - 76 ) ^ 2"}; // 25 17 67 76 - 2 ^ * +
    std::string string{"( ( 2 + 3 ) * 4 - 5 ) / ( 6 - 4 ) ^ 2"}; // 2 3 + 4 * 5 - 6 4 - 2 ^ /
    //std::string string{"2 ^ 3 ^ 2"}; // 2 3 2 ^ ^
    std::cout << '\n' << RPN(string) << '\n';
    std::cout << "result: " << RPN_calculate(RPN(string));
}

TNT
void init(Stack<T>& stack, const size_t& size, const float& factor) {
    stack.size = size;
    stack.top = 0;
    if(factor < 1) {
        return;
    }
    stack.resize_factor = factor;
    stack.st = new T[size];
}

TNT
void clear(Stack<T>& stack) {
    stack.size = 0;
    stack.top = 0;
    delete[] stack.st;
    stack.st = nullptr;
}

TNT
bool is_empty(const Stack<T>& stack) {
    return stack.top == 0;
}

TNT
void push(Stack<T>& stack, const T& elmnt) {
    if(stack.top == stack.size) {
        size_t new_size = stack.size * stack.resize_factor;
        resize(stack, new_size);
    }
    stack.st[stack.top++] = elmnt;
}

TNT
T pop(Stack<T>& stack) {
    if(is_empty(stack)) {
        throw "Errrrr...";
    }
    return stack.st[--stack.top];
}

TNT
T top(const Stack<T>& stack) {
    if(is_empty(stack)) {
        return "";
    }
    return stack.st[stack.top - 1];
}

size_t tkns_mnt(const std::string& str) {
    std::istringstream ss(str);
    std::string temp{""};
    size_t tkns_mnt{0};
    while(ss >> temp) {
        ++tkns_mnt;
    }
    return tkns_mnt;
}

std::string* tokenize(const std::string& str) {
    size_t tkns = tkns_mnt(str);
    std::string* tkns_arr = new std::string[tkns];
    std::istringstream ss(str);
    tkns = 0;
    std::string temp{""};
    while(ss >> temp) {
        tkns_arr[tkns++] = temp;
    }
    return tkns_arr;
}

std::string RPN(const std::string& str) {
    size_t tkns = tkns_mnt(str);
    Stack<std::string> stack;
    init(stack, 2, 1.5f);
    std::string* tkn_arr = tokenize(str);
    std::string out{""};
    for(size_t i{0}; i < tkns; ++i) {
        if(alpha_prior(tkn_arr[i]) == -1) {
            out = out + tkn_arr[i] + ' ';
            continue;
        }
        if(tkn_arr[i] == "(") {
            push(stack, tkn_arr[i]);
            continue;
        }
        if(tkn_arr[i] == ")") {
            while(top(stack) != "(") {
                out = out + pop(stack) + ' ';
            }
            pop(stack);
            continue;
        }
        while(!is_empty(stack) && alpha_prior(top(stack)) >= alpha_prior(tkn_arr[i])) {
            out = out + pop(stack) + ' ';
        }
        push(stack, tkn_arr[i]);
    }
    while(!is_empty(stack)) {
        out = out + pop(stack) + ' ';
    }
    clear(stack);
    return out;
}

int16_t alpha_prior(const std::string& alpha) {
    if((alpha.size() >= 2 && (alpha[0] == '-' || (alpha[0] >= '0' && alpha[0] <= '9'))) || (alpha[0] >= '0' && alpha[0] <= '9')) {
        return -1;
    }
    if(alpha == "+" || alpha == "-") {
        return 1;
    }
    if(alpha == "*" || alpha == "/") {
        return 2;
    }
    if(alpha == "^") {
        return 3;
    }
    return 0;
}

double RPN_calculate(const std::string& str) {
    size_t tkns = tkns_mnt(str);
    Stack<double> stack;
    init(stack, 2, 1.5f);
    std::string* tkn_arr = tokenize(str);
    double temp1{0};
    double temp2{0};
    for(size_t i{0}; i < tkns; ++i) {
        if(alpha_prior(tkn_arr[i]) == -1) {
            push(stack, std::stod(tkn_arr[i]));
            continue;
        }
        if(tkn_arr[i] == "+") {
            temp1 = pop(stack);
            temp2 = pop(stack);
            push(stack, temp2 + temp1);
            continue;
        }
        if(tkn_arr[i] == "-") {
            temp1 = pop(stack);
            temp2 = pop(stack);
            push(stack, temp2 - temp1);
            continue;
        }
        if(tkn_arr[i] == "*") {
            temp1 = pop(stack);
            temp2 = pop(stack);
            push(stack, temp2 * temp1);
            continue;
        }
        if(tkn_arr[i] == "/") {
            temp1 = pop(stack);
            temp2 = pop(stack);
            push(stack, temp2 / temp1);
            continue;
        }
        if(tkn_arr[i] == "^") {
            temp1 = pop(stack);
            temp2 = pop(stack);
            push(stack, std::pow(temp2, temp1));
            continue;
        }
    }
    temp1 = pop(stack);
    clear(stack);
    return temp1;
}

TNT 
void resize(Stack<T>& stack, const size_t& new_size) {
    if(stack.size == new_size) {
        return;
    }
    size_t tocopy = std::min(stack.size, new_size);
    T* arr_buff = new T[new_size];
    for(size_t i{0}; i < tocopy; ++i) {
        arr_buff[i] = stack.st[i];
    }
    delete[] stack.st;
    stack.size = new_size;
    stack.st = arr_buff;
    if(stack.top > new_size) {
        stack.top = new_size;
    }
}