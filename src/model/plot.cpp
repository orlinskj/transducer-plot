#include "plot.h"

#include <algorithm>

ac::Plot::Plot() {}
ac::Plot::Plot(const std::string& name) : name_(name) { }

void Plot::add_function(const ac::Function& f)
{
    functions_.push_back(f);
}

bool ac::Plot::remove_function(const ac::Function& f)
{
    auto it = std::find(functions_.begin(), functions_.end(), f);
    if (it != functions_.end())
    {
        functions_.erase(it);
        return true;
    }
    else
    {
        return false;
    }
}

bool ac::Plot::find_function(const ac::Function& f) const
{
    auto it = std::find(functions_.begin(), functions_.end(), f);
    if (it != functions_.end())
        return true;
    else
        return false;
}

const std::string& ac::Plot::get_name() const
{
    return name_;
}
