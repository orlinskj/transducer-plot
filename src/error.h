#ifndef ERROR_H
#define ERROR_H

#include <stdexcept>

class Error : public std::logic_error
{
public:
    Error(const std::string& what);
};

#endif // ERROR_H
