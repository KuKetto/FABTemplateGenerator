#ifndef LIBRARYEXCEPTION_H
#define LIBRARYEXCEPTION_H

#include <stdexcept>

class LibraryException : std::exception
{
    std::string message;
public:
    LibraryException(const std::string& function,
                     const std::string& library,
                     const std::string& error_message);

    const char* what() const noexcept override;
};

#endif // LIBRARYEXCEPTION_H
