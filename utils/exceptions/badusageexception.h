#ifndef BADUSAGEEXCEPTION_H
#define BADUSAGEEXCEPTION_H

#include <stdexcept>

class BadUsageException : std::exception
{
    std::string message;
public:
    BadUsageException(const std::string& function,
                      const std::string& reason,
                      const std::string& provided_value = "",
                      const std::string& provided_value_type = "",
                      const std::string& expected_value_type = "");

    const char* what() const noexcept override;
};

#endif // BADUSAGEEXCEPTION_H
