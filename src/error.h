#ifndef ERROR_H
#define ERROR_H

#include <stdexcept>

class Error : public std::logic_error
{
public:
    enum class Type {
        Info,
        Error
    };

    Error(const std::string& what, Type type = Type::Error);

    Type type() const;
protected:
    Type type_;
};

#endif // ERROR_H
