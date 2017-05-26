#include "error.h"

Error::Error(const std::string& what) : std::logic_error(what) {}
