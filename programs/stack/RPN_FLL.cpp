#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

#define Type template <class T>

Type
struct Node
{
    Node<T>* next {nullptr};
    T obj{};
};

Type
struct forwardlinkedlist
{
    Node<T>* top{nullptr};
};

Type
void clear(forwardlinkedlist<T>&);

Type
bool is_empty(const forwardlinkedlist<T>&);

Type
void push(forwardlinkedlist<T>&, const T&);

Type
T pop(forwardlinkedlist<T>&);

Type
T top(const forwardlinkedlist<T>&); 
size_t tkns_mnt(const std::string&);
std::string* tokenize(const std::string&);
std::string RPN(const std::string&);
int16_t alpha_prior(const std::string&);
double RPN_calculate(const std::string&);


int main() {
    //std::string string{"25 + 17 * ( 67 - 76 ) ^ 2"}; // 25 17 67 76 - 2 ^ * +  = 1402
    //std::string string{"( ( 2 + 3 ) * 4 - 5 ) / ( 6 - 4 ) ^ 2"}; // 2 3 + 4 * 5 - 6 4 - 2 ^ /   = 3.75
    //std::string string{"2 ^ 3 ^ 2"}; // 2 3 2 ^ ^
    std::string string{"3 + sin ( 2 * 4 ) * cos ( 5 )"}; // 3 2 4 * sin 5 cos * +    = 3.28064
    std::cout << '\n' << RPN(string) << '\n';
    std::cout << "result: " << RPN_calculate(RPN(string));
}

Type
void clear(forwardlinkedlist<T>& stack) {
    while(!is_empty(stack)) {
        pop(stack);
    }
}

Type
bool is_empty(const forwardlinkedlist<T>& stack) {
    return stack.top == nullptr;
}

Type
void push(forwardlinkedlist<T>& stack, const T& elmnt) {
    Node<T>* temp = new Node<T>;
    temp->obj = elmnt;
    temp->next = stack.top;
    stack.top = temp;
}

Type
T pop(forwardlinkedlist<T>& stack) {
    if(is_empty(stack)) {
        throw "ERR";
    }
    Node<T>* temp = stack.top;
    stack.top = stack.top->next;
    T val = temp->obj;
    delete temp;
    return val;
}

Type
T top(const forwardlinkedlist<T>& stack) {
    if(is_empty(stack)) {
        return {};
    }
    return stack.top->obj;
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
    forwardlinkedlist<std::string> stack{};
    std::string* tkn_arr = tokenize(str);
    std::string out{""};
    for(size_t i{0}; i < tkns; ++i) {
        if(alpha_prior(tkn_arr[i]) == -1) {
            out = out + tkn_arr[i] + ' ';
            continue;
        }
        if(tkn_arr[i] == "(" || tkn_arr[i] == "sin" || tkn_arr[i] == "cos") {
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
    if((alpha.size() > 2) && (alpha == "sin" || alpha == "cos")){
        return 10;
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
    forwardlinkedlist<double> stack;
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
        if(tkn_arr[i] == "sin") {
            temp1 = pop(stack);
            push(stack, sin(temp1));
            continue;
        }
        if(tkn_arr[i] == "cos") {
            temp1 = pop(stack);
            push(stack, cos(temp1));
            continue;
        }
    }
    temp1 = pop(stack);
    clear(stack);
    return temp1;
}