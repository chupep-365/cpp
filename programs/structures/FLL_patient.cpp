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

enum class State : u_int32_t { CRITICAL = 400000, HARD = 300000, MEDIUM = 200000, EASY = 100000};

struct Patient {
    State state{};
    std::string name{};
    u_int16_t age{};
    u_int16_t pos{};
};

template <class T>
void push(Priority_queue<T>&, const T&, u_int32_t (*prior_calc)(const T&));

template <class T>
T pop(Priority_queue<T>&);

template <class T>
bool empty(const Priority_queue<T>&);

u_int32_t patient_prior(const Patient&);
void input(Patient&);
void output(const Patient&);

int main() {
    Priority_queue<Patient> prior_q;
    for(size_t i{0}; i < 5; ++i) {
        Patient temp{};
        input(temp);
        push(prior_q, temp, patient_prior);
    }
    std::cout << '\n';
    while(!empty(prior_q)) {
        output(pop(prior_q));
    }
}

template <class T>
void push(Priority_queue<T>& pq, const T& obj, u_int32_t (*prior_calc)(const T&)) {
    u_int32_t priority = prior_calc(obj);   // func calculates priority of object
    if(pq.top == nullptr) {
        pq.top = new Node<T>{obj, nullptr, nullptr, priority};
        return;
    }
    Node<T>* temp = pq.top;
    bool in_between{false};
    Node<T>* new_node = new Node<T>{obj, nullptr, nullptr, priority};
    while(true) {
        if(temp->prior >= new_node->prior) {
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
    if(!empty(pq)){ 
        pq.top->prev = nullptr;
    }
    delete temp;
    return obj;
}

template <class T>
bool empty(const Priority_queue<T>& pq) {
    return pq.top == nullptr;
}

u_int32_t patient_prior(const Patient& obj) {
    return (u_int32_t)obj.state - obj.pos;
}

void input(Patient& obj) {
    std::cin >> obj.name;
    std::cin >> obj.age;
    std::string str{};
    std::cin >> str;
    if(str == "CRITICAL"){
        obj.state = State::CRITICAL;
    }
    if(str == "HARD"){
        obj.state = State::HARD;
    }
    if(str == "MEDIUM"){
        obj.state = State::MEDIUM;
    }
    if(str == "EASY"){
        obj.state = State::EASY;
    }
    std::cin >> obj.pos;
}

void output(const Patient& obj) {
    std::string state{};
    switch (obj.state)
    {
    case State::CRITICAL :
        state = "CRITICAL";
        break;
    case State::HARD :
        state = "HARD";
        break;
    case State::MEDIUM :
        state = "MEDIUM";
        break;
    case State::EASY :
        state = "EASY";
        break;
    default:
        throw "ERROR NO SUCH STATE";
        break;
    }
    std::cout << obj.name << ' ' << obj.age << ' ' << state << '\n';
}