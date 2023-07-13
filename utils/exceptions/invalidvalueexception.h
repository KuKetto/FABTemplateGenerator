#ifndef INVALIDVALUEEXCEPTION_H
#define INVALIDVALUEEXCEPTION_H

#include <stdexcept>

class InvalidValueException : std::exception
{
    std::string message;
public:
    InvalidValueException(const std::string& function,
                          const std::string& expected_value,
                          const std::string& input_value);

    const char* what() const noexcept override;
};

#endif // INVALIDVALUEEXCEPTION_H
