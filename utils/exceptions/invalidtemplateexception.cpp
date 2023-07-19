#include "invalidtemplateexception.h"

InvalidTemplateException::InvalidTemplateException(const std::string &function, const std::string &reason)
{
    message = "InvalidTemplateException invoked at: " + function + " - reason: " + reason;
}

const char *InvalidTemplateException::what() const noexcept
{
    return message.c_str();
}
