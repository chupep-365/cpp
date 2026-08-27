#include <iostream>

template <class T>
struct PQ{
    T* arr;
    bool(*comp)(const T&, const T&);
    size_t top{-1};
    size_t max_index{};
    float resize_factor{};
};

template <class T>
PQ<T> init(const T&, bool(*func)(const T&, const T&), const float&);

template <class T>
void push(PQ<T>&, const T&);

template <class T>
void resize(PQ<T>&, const size_t&);

int main() {



/////////////////////////////////////////////////////////////////////


}

template <class T>
PQ<T> init(bool(*func)(const T&, const T&), const float& rf) {
    PQ<T> queue;
    queue.arr = new T[2];
    queue.comp = func;
    queue.max_index = 1;
    queue.resize_factor = rf;
    return queue;
}

template <class T>
void push(PQ<T>& pq, const T& obj) {
    if(pq.top == (size_t)-1) {
        ++pq.top;
        pq.arr[pq.top] = obj;
        return;
    }
    if(pq.top == pq.max_index) {
        resize(pq, (pq.max_index + 1) * pq.resize_factor);
    }
    
}

template <class T>
void resize(PQ<T>& pq, const size_t& new_size) {
    if(pq.max_index == new_size - 1) {
        return;
    }
    size_t tocopy = std::min(pq.max_index + 1, new_size);
    T* arr_buff = new T[new_size];
    for(size_t i{0}; i < tocopy; ++i) {
        arr_buff[i] = pq.arr[i];
    }
    delete[] pq.arr;
    pq.max_index = new_size - 1;
    pq.arr = arr_buff;
    if(pq.top > new_size) {
        pq.top = new_size - 1;
    }
}