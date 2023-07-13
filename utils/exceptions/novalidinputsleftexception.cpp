#include "novalidinputsleftexception.h"

NoValidInputsLeftException::NoValidInputsLeftException(const std::string &function, const std::string &reason)
{
    message = "NoValidInputsLeftException invoked at " + function + ": No more valid inputs left, " + reason;
}

const char *NoValidInputsLeftException::what() const noexcept
{
    return message.c_str();
}
