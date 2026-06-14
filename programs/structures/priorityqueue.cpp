#include <iostream>
#include <string>

template <class T>
struct Node{
    T obj{};
    Node<T>* next{nullptr};
    Node<T>* prev{nullptr};
    u_int32_t prior{};
};

template <class T>
struct Priority_queue{
    Node<T>* top{nullptr};
};

template <class T>
void push(Priority_queue<T>&, const T&);

template <class T>
T pop(Priority_queue<T>&);

u_int32_t func();

int main() {
    Priority_queue<std::string> prior_q;
    push(prior_q, (std::string)"First");    // for tests
    push(prior_q, (std::string)"Second");   
    pop(prior_q);
    push(prior_q, (std::string)"Third");
    push(prior_q, (std::string)"Fourth");
    pop(prior_q);
    pop(prior_q);
    std::cout << "YAY";
}

u_int32_t func() {
    u_int32_t p{};
    std::cin >> p;
    return p;
}

template <class T>
void push(Priority_queue<T>& pq, const T& obj) {
    u_int32_t priority = func(); // func calculates priority of object
    if(pq.top == nullptr) {
        pq.top = new Node<T>{obj, nullptr, nullptr, priority};
        return;
    }
    Node<T>* temp = pq.top;
    bool in_between{false};
    Node<T>* new_node = new Node<T>{obj, nullptr, nullptr, priority};
    while(true) {
        if(temp->prior > new_node->prior) {
            if(temp->next != nullptr) {
                temp = temp->next;
                in_between = true;
                continue;
            }
            Node<T>& anchor = *temp;
            new_node->prev = &anchor;
            anchor.next = new_node;
            break;
        } 
        Node<T>& anchor = *temp;
        if(in_between) {
            new_node->next = &anchor;
            new_node->prev = anchor.prev;
            anchor.prev->next = new_node;
            anchor.prev = new_node;
            break;
        }
        new_node->next = &anchor;
        anchor.prev = new_node;
        pq.top = new_node;
        break;
    }
    return;
}

template <class T>
T pop(Priority_queue<T>& pq) {
    if(pq.top == nullptr) {
        throw "ERR";
    }
    Node<T>* temp = pq.top;
    T obj = temp->obj;
    pq.top = temp->next;
    if(pq.top != nullptr) {
        pq.top->prev = nullptr;
    }
    delete temp;
    return obj;
}