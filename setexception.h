#ifndef SETEXCEPTION_H
#define SETEXCEPTION_H

#include <string>
#include <exception>

class SetException : public std::exception{
private:
    std::string message;
public:
    SetException(const std::string& message) : message(message) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif // SETEXCEPTION_H
