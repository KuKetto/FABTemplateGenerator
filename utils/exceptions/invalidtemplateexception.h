#ifndef INVALIDTEMPLATEEXCEPTION_H
#define INVALIDTEMPLATEEXCEPTION_H

#include <stdexcept>

class InvalidTemplateException : std::exception
{
    std::string message;
public:
    InvalidTemplateException(const std::string& function,
                             const std::string& image_reading_exception);

    const char* what() const noexcept override;
};

#endif // INVALIDTEMPLATEEXCEPTION_H
