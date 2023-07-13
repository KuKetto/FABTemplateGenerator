#include "pathnotfoundexception.h"

PathNotFoundException::PathNotFoundException(const std::string &function, const std::string &file_path)
{
    message = "FileNotFound invoked at " + function +
              ": Path not found. Given path: " + file_path;
}

const char *PathNotFoundException::what() const noexcept
{
    return message.c_str();
}
