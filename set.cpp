#include "set.h"

template<typename T>
set<T>::set() : data(nullptr), len(0), capacity(0) {}

template<typename T>
set<T>::set(const set<T>& s) : data(nullptr), len(0), capacity(0){ //2
    copy(s);
}

template<typename T>
set<T>::set(set <T>&& s) noexcept : data(s.data), len(s.len), capacity(s.capacity) { //4
    s.data = nullptr;
    s.len = 0;
    s.capacity = 0;
}

template<typename T>
set<T>::set(std::initializer_list<T> lst) : data(nullptr), len(0), capacity(0){
    for (const T& elem : lst)
        add(elem);
}

template<typename T>
set<T>::~set() { //1
    delete[] data;
}

template<typename T>
set<T>& set<T>::operator =(const set<T>& lst){ //3
    if (this != &lst) {
        set<T> tmp(lst);
        swap(tmp);
    }
    return *this;
}

template<typename T>
set<T>& set<T>::operator =(set<T>&& lst) noexcept{ //5
    if (this != &lst){
        delete[] data;
        data = lst.data;
        len = lst.len;
        capacity = lst.capacity;
        lst.data = nullptr;
        lst.len = 0;
        lst.capacity = 0;
    }
    return *this;
}

template<typename T>
void set<T>::copy(const set<T>& s){
    if (this != &s){
        T* newData = nullptr;
        if (s.len > 0) {
            newData = new T[s.capacity];
            for (int i = 0; i < s.len; ++i) {
                newData[i] = s.data[i];
            }
        }
        delete[] data;
        data = newData;
        capacity = s.capacity;
        len = s.len;
    }
}

template<typename T>
void set<T>::reallocate(int required){
    if (required > capacity){
        int newCapacity = (capacity == 0) ? BEGIN_CAPACITY : capacity * 2;
        if (newCapacity < required)
            newCapacity = required;
        T* newData = new T[newCapacity];
        for (int i = 0; i < len; ++i)
            newData[i] = data[i];
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }
}

template<typename T>
int set<T>::getLength() const{
    return len;
}

template<typename T>
bool set<T>::contains(const T& elem){
    bool containsElem = false;
    for (int i = 0; i < len && !containsElem; ++i){
        if (data[i] == elem)
            containsElem = true;
    }
    return containsElem;
}

template<typename T>
void set<T>::add(const T& elem){
    if (!contains(elem)){
        reallocate(len + 1);
        data[len++] = elem;
    }
}

template<typename T>
T* set<T>::toArray() const{
    T* arr = new T[len];
    for (int i = 0; i < len; ++i)
        arr[i] = data[i];
    return arr;
}

template<typename T>
set<T>& set<T>::unionWith(const set<T>& s){
    for(int i = 0; i < s.len; ++i)
        add(s.data[i]);
    return *this;
}

template<typename T>
set<T>& set<T>::intersection(const set<T>& s){
    int newLen = 0;
    for (int i = 0; i < len; ++i)
        if (s.contains(data[i]))
            data[newLen++] = data[i];
    len = newLen;
    return *this;
}

template<typename T>
set<T>& set<T>::subtract(const set<T>& s){
    int newLen = 0;
    for (int i = 0; i < len; ++i)
        if (!s.contains(data[i]))
            data[newLen++] = data[i];
    len = newLen;
    return *this;
}

template<typename T>
void set<T>::swap(set<T>& other) noexcept{
    std::swap(data, other.data);
    std::swap(len, other.len);
    std::swap(capacity, other.capacity);
}
