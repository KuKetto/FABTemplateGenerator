#include "invalidtemplateexception.h"

InvalidTemplateException::InvalidTemplateException(const std::string &function, const std::string &image_reading_exception)
{
    message = "In the function: " + function + " exception happened. " + image_reading_exception;
}

const char *InvalidTemplateException::what() const noexcept
{
    return message.c_str();
}
