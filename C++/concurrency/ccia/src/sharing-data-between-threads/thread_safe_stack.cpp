#include <iostream>
#include <stdexcept>
#include <string>

class EmptyStack : public std::exception
{
private:
    std::string message;

public:
    // Constructor to set a custom message
    EmptyStack(const std::string &msg) : message(msg) {}

    // 2. Override the what() method with the correct signature
    const char *what() const noexcept override
    {
        // Return the custom error message
        return message.c_str();
    }
};