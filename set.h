#ifndef SET_H
#define SET_H

#include <iostream>
#include "setexception.h"
#include "iterator.h"

template<typename T>
class Iterator;

template<typename T>
class set {
private:
    enum class State {
        EMPTY,
        OCCUPIED,
        DELETED
    };
    struct Slot {
        T value;
        State state = State::EMPTY;
    };
    Slot* data;
    int len;
    int capacity;
    static constexpr int BEGIN_CAPACITY = 16;
    static constexpr double LOAD_FACTOR = 0.7;
    void reallocate(int required);
    void copy(const set<T>& s);
    int getHash(const T& elem, int capacity) const;
    int findSlot(const T& elem) const;
    friend class Iterator<T>;
public:
    set();
    set(const set<T>& s);
    set(set <T>&& s) noexcept;
    explicit set(std::initializer_list<T> lst);
    ~set();
    set<T>& operator =(const set<T>& lst);
    set<T>& operator =(set<T>&& lst) noexcept;
    int getLength() const;
    int getCapacity() const;
    bool contains(const T& elem) const;
    void add(const T& elem);
    void swap(set<T>& other) noexcept;
    void remove(const T& elem);
    T* toArray() const;
    set<T>& unionWith(const set<T>& s);
    set<T>& intersection(const set<T>& s);
    set<T>& subtract(const set<T>& s);
    set<T>& operator /=(const set<T>& s);
    set<T>& operator +=(const set<T>& s);
    set<T>& operator *=(const set<T>& s);
    void clear();

    template<typename _T>
    friend std::ostream& operator <<(std::ostream& os, const set<_T>& lst);

    template<typename _T>
    friend set<_T> operator +(const set<_T>& s1, const set<_T>& s2);

    template<typename _T>
    friend set<_T> operator *(const set<_T>& s1, const set<_T>& s2);

    template<typename _T>
    friend set<_T> operator /(const set<_T>& s1, const set<_T>& s2);

    Iterator<T> iteratorBegin() const;
    Iterator<T> iteratorEnd() const;
};

template<typename T>
set<T>::set() : data(nullptr), len(0), capacity(0) {}

template<typename T>
set<T>::set(const set<T>& s) : data(nullptr), len(0), capacity(0) { //2
    copy(s);
}

template<typename T>
set<T>::set(set <T>&& s) noexcept : data(s.data), len(s.len), capacity(s.capacity) { //4
    s.data = nullptr;
    s.len = 0;
    s.capacity = 0;
}

template<typename T>
set<T>::set(std::initializer_list<T> lst) : data(nullptr), len(0), capacity(0) {
    for (const T& elem : lst)
        add(elem);
}

template<typename T>
set<T>::~set() { //1
    delete[] data;
}

template<typename T>
set<T>& set<T>::operator =(const set<T>& lst) { //3
    if (this != &lst) {
        set<T> tmp(lst);
        swap(tmp);
    }
    return *this;
}

template<typename T>
set<T>& set<T>::operator =(set<T>&& lst) noexcept { //5
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
void set<T>::copy(const set<T>& s) {
    if (this != &s) {
        delete[] data;
        data = nullptr;
        if (s.capacity > 0) {
            data = new Slot[s.capacity];
            for (int i = 0; i < s.capacity; ++i)
                data[i] = s.data[i];
        }
        capacity = s.capacity;
        len = s.len;
    }
}

template<typename T>
void set<T>::reallocate(int required) {
    if (required > capacity){
        int newCapacity = (capacity == 0) ? BEGIN_CAPACITY : capacity * 2;
        if (newCapacity < required)
            newCapacity = required;
        Slot* newData = new Slot[newCapacity];
        for (int i = 0; i < capacity; ++i)
            if (data[i].state == State::OCCUPIED){
                int newIndex = getHash(data[i].value, newCapacity);
                while (newData[newIndex].state == State::OCCUPIED)
                    newIndex = (newIndex + 1) % newCapacity;
                newData[newIndex].value = data[i].value;
                newData[newIndex].state = State::OCCUPIED;
            }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }
}

template<typename T>
int set<T>::getLength() const {
    return len;
}

template<typename T>
int set<T>::getCapacity() const {
    return capacity;
}

template<typename T>
int set<T>::getHash(const T& elem, int capacity) const {
    return std::hash<T>{}(elem) % capacity;
}

template<typename T>
int set<T>::findSlot(const T& elem) const {
    int result = -1;
    if (capacity > 0){
        int index = getHash(elem, capacity), firstDeleted = -1, startIndex = index;
        do {
            if (data[index].state == State::OCCUPIED && data[index].value == elem)
                result = index;
            else if (data[index].state == State::DELETED && firstDeleted == -1)
                firstDeleted = index;
            else if (data[index].state == State::EMPTY)
                result  = (firstDeleted != -1) ? firstDeleted : index;
            index = (index + 1) % capacity;
        } while (index != startIndex && result == -1);
        if (result == -1)
            result = firstDeleted;
    }
    return result;
}

template<typename T>
bool set<T>::contains(const T& elem) const {
    int index = findSlot(elem);
    return (index != -1 && data[index].state == State::OCCUPIED && data[index].value == elem) ? true : false;
}

template<typename T>
void set<T>::add(const T& elem) {
    if (capacity == 0 || (len >= capacity * LOAD_FACTOR)) {
        reallocate(capacity == 0 ? BEGIN_CAPACITY : capacity * 2);
    }
    int index = findSlot(elem);
    if (index != -1 && data[index].state != State::OCCUPIED) {
        data[index].value = elem;
        data[index].state = State::OCCUPIED;
        len++;
    }
}

template<typename T>
void set<T>::remove(const T& elem) {
    int index = findSlot(elem);
    if (index == -1 || data[index].state != State::OCCUPIED || data[index].value != elem)
        throw SetException("Element not found in set");
    else{
        data[index].state = State::DELETED;
        len--;
    }
}

template<typename T>
T* set<T>::toArray() const {
    T* arr = new T[len];
    int j = 0;
    for (int i = 0; i < capacity; ++i)
        if (data[i].state == State::OCCUPIED)
            arr[j++] = data[i].value;
    return arr;
}

template<typename T>
set<T>& set<T>::unionWith(const set<T>& s) {
    for (int i = 0; i < s.capacity; ++i)
        if (s.data[i].state == State::OCCUPIED)
            add(s.data[i].value);
    return *this;
}

template<typename T>
set<T>& set<T>::intersection(const set<T>& s) {
    set<T> result;
    for (int i = 0; i < capacity; ++i)
        if (data[i].state == State::OCCUPIED && s.contains(data[i].value))
            result.add(data[i].value);
    this->swap(result);
    return *this;
}

template<typename T>
set<T>& set<T>::subtract(const set<T>& s) {
    set<T> result;
    for (int i = 0; i < capacity; ++i)
        if (data[i].state == State::OCCUPIED && !s.contains(data[i].value))
            result.add(data[i].value);
    this->swap(result);
    return *this;
}

template<typename _T>
std::ostream& operator <<(std::ostream& os, const set<_T>& lst) {
    os << "{ ";
    for (int i = 0; i < lst.capacity; ++i) {
        if (lst.data[i].state == set<_T>::State::OCCUPIED)
            os << lst.data[i].value << " ";
    }
    os << " }";
    return os;
}

template<typename T>
set<T>& set<T>::operator +=(const set<T>& s) {
    return unionWith(s);
}

template<typename T>
set<T>& set<T>::operator *=(const set<T>& s) {
    return intersection(s);
}

template<typename T>
set<T>& set<T>::operator /=(const set<T>& s) {
    return subtract(s);
}

template<typename T>
void set<T>:: clear() {
    delete[] data;
    data = nullptr;
    len = 0;
    capacity = 0;
}

template<typename _T>
set<_T> operator +(const set<_T>& s1, const set<_T>& s2) {
    set<_T> result(s1);
    result.unionWith(s2);
    return result;
}

template<typename _T>
set<_T> operator *(const set<_T>& s1, const set<_T>& s2) {
    set<_T> result(s1);
    result.intersection(s2);
    return result;
}

template<typename _T>
set<_T> operator /(const set<_T>& s1, const set<_T>& s2) {
    set<_T> result(s1);
    result.subtract(s2);
    return result;
}

template<typename T>
void set<T>::swap(set<T>& other) noexcept {
    std::swap(data, other.data);
    std::swap(len, other.len);
    std::swap(capacity, other.capacity);
}

template<typename T>
Iterator<T> set<T>::iteratorBegin() const {
    int first = 0;
    while (first < capacity && data[first].state != State::OCCUPIED)
        first++;
    return Iterator<T>(*this, first);
}

template<typename T>
Iterator<T> set<T>::iteratorEnd() const {
    return Iterator<T>(*this, capacity);
}

#endif // SET_H
