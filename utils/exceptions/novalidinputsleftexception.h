#ifndef NOVALIDINPUTSLEFTEXCEPTION_H
#define NOVALIDINPUTSLEFTEXCEPTION_H

#include <stdexcept>

class NoValidInputsLeftException : std::exception
{
    std::string message;
public:
    NoValidInputsLeftException(const std::string& function,
                               const std::string& reason);

    const char* what() const noexcept override;
};

#endif // NOVALIDINPUTSLEFTEXCEPTION_H
