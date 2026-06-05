#ifndef ITERATOREXCEPTION_H
#define ITERATOREXCEPTION_H

#include <string>
#include <exception>

class IteratorException : public std::exception{
private:
    std::string message;
public:
    IteratorException(const std::string& message) : message(message) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif // ITERATOREXCEPTION_H
