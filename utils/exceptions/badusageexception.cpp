#include "badusageexception.h"

BadUsageException::BadUsageException(const std::string &function, const std::string &reason, const std::string &provided_value, const std::string &provided_value_type, const std::string &expected_value_type)
{
    message = "BadUsageException invoked at " + function + ". Unexpected behaviour: "
              + reason + ".\nThe following value was provided: " + provided_value + " AKA ("
              + provided_value_type + ") - Which is not the expected " + expected_value_type + "type";
}

const char *BadUsageException::what() const noexcept
{
    return message.c_str();
}
