#include "dividebyzero.h"

DivideByZero::DivideByZero(const std::string &function)
{
    message = "DivideByZero invoked at " + function + ": tried to divide with 0";
}

const char *DivideByZero::what() const noexcept
{
    return message.c_str();
}
