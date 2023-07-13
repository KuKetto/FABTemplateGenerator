#ifndef PATHNOTFOUNDEXCEPTION_H
#define PATHNOTFOUNDEXCEPTION_H

#include <stdexcept>

class PathNotFoundException : std::exception
{
    std::string message;
public:
    PathNotFoundException(const std::string& function,
                 const std::string& file_path);

    const char* what() const noexcept override;
};

#endif // PATHNOTFOUNDEXCEPTION_H
