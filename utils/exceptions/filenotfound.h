#ifndef FILENOTFOUND_H
#define FILENOTFOUND_H

#include <stdexcept>

class FileNotFound : std::exception
{
    std::string message;
public:
    FileNotFound(const std::string& function,
                 const std::string& file_path);

    const char* what() const noexcept override;
};

#endif // FILENOTFOUND_H
