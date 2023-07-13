#include "filenotfound.h"

FileNotFound::FileNotFound(const std::string &function, const std::string &file_path)
{
    message = "FileNotFound invoked at " + function +
              ": File not found. Given file path: " + file_path;
}

const char *FileNotFound::what() const noexcept
{
    return message.c_str();
}
