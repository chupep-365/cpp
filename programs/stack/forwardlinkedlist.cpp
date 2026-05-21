#include <iostream>
#include <string>

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
void push(forwardlinkedlist<T>& stack, const T& elmnt) {
    Node<T>* temp = new Node<T>;
    temp->obj = elmnt;
    temp->next = stack.top;
    stack.top = temp;
}

Type
T pop(forwardlinkedlist<T>& stack) {
    if(stack.top == nullptr) {
        throw "ERR";
    }
    Node<T>* temp = stack.top;
    stack.top = stack.top->next;
    T val = temp->obj;
    delete temp;
    return val;
}

Type
bool is_empty(const forwardlinkedlist<T>& stack) {
    return stack.top == nullptr;
}

Type
T top(const forwardlinkedlist<T>& stack) {
    if(is_empty(stack)) {
        return {};
    }
    return stack.top->obj;
}

Type
void clear(forwardlinkedlist<T>& stack) {
    while(!is_empty(stack)) {
        pop(stack);
    }
}


int main() {
    forwardlinkedlist<int> stack{nullptr};
    push(stack, 1);
    push(stack, 2);
    std::cout << pop(stack) << '\n';
    push(stack, 23);
    std::cout << pop(stack) << ' ' << pop(stack) << '\n';
    push(stack, 999);
    push(stack, 888);
    clear(stack);
}
