#include "invalidvalueexception.h"

InvalidValueException::InvalidValueException(const std::string function, const std::string &expected_value, const std::string &input_value)
{
    this->message = "InvalidValueException invoked at " + function +
                    ": A value " + expected_value +
                    " was expected but instead " + input_value + " was provided";
}

const char *InvalidValueException::what() const noexcept
{
    return message.c_str();
}
