#ifndef ITERATOR_H
#define ITERATOR_H

#include "iteratorexception.h"

template<typename T>
class set;

template<typename T>
class Iterator {
public:
    Iterator(const set<T>& containerObj, int beginIndex = 0);
    Iterator<T> next();
    T value() const;
    bool isEnd() const;
    Iterator<T> &operator++();
    T &operator*() const;
    bool operator ==(const Iterator<T> &b) const;
    bool operator !=(const Iterator<T> &b) const;
private:
    const set<T>* container;
    int index;
};

template<typename T>
Iterator<T>::Iterator(const set<T>& containerObj, int beginIndex) : container(&containerObj), index(beginIndex) {}

template<typename T>
Iterator<T> Iterator<T>::next() {
    if (index >= container->getCapacity())
        throw IteratorException("Iterator is already at end");
    ++index;
    while (index < container->getCapacity() && container->data[index].state != set<T>::State::OCCUPIED)
        ++index;
    return *this;
}

template<typename T>
T Iterator<T>::value() const{
    if (index >= container->getCapacity())
        throw IteratorException("Iterator is at end");
    return container->data[index].value;
}

template<typename T>
bool Iterator<T>::isEnd() const {
    return index >= container->getCapacity();
}

template<typename T>
Iterator<T>& Iterator<T>::operator++() {
    this->next();
    return *this;
}

template<typename T>
T& Iterator<T>:: operator*() const {
    if (index >= container->getCapacity())
        throw IteratorException("Iterator is at end");
    return container->data[index].value;
}

template<typename T>
bool Iterator<T>::operator ==(const Iterator<T> &b) const {
    return container == b.container && index == b.index;
}

template<typename T>
bool Iterator<T>::operator !=(const Iterator<T> &b) const {
    return !(*this == b);
}

#endif // ITERATOR_H
