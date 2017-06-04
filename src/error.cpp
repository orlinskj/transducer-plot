#include "error.h"

Error::Error(const std::string& what, Error::Type type) : std::logic_error(what), type_(type) {}

Error::Type Error::type() const
{
    return type_;
}
