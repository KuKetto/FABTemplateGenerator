#ifndef DIVIDEBYZERO_H
#define DIVIDEBYZERO_H

#include <stdexcept>

class DivideByZero : std::exception
{
    std::string message;
public:
    DivideByZero(const std::string& function);

    const char* what() const noexcept override;
};

#endif // DIVIDEBYZERO_H
