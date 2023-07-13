#include "libraryexception.h"

LibraryException::LibraryException(const std::string &function, const std::string &library, const std::string &error_message)
{
    message = "LibraryException invoked at " + function +
              ": " + library + " library caused an exception. Error message: " + error_message;
}

const char *LibraryException::what() const noexcept
{
    return message.c_str();
}
