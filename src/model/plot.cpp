#include "plot.h"

#include <algorithm>

ac::Plot::Plot() {}
ac::Plot::Plot(const std::string& name) : name_(name) { }

ac::Plot_ptr ac::Plot::make_ptr(ac::Plot &&plot)
{
    return std::make_shared<ac::Plot>(plot);
}

void ac::Plot::add_function(const ac::Function& f)
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

int ac::Plot::function_count() const
{
    return functions_.size();
}

const ac::Function& ac::Plot::function_at(int index) const
{
    return functions_.at(index);
}

std::string ac::Plot::to_string() const
{
    return get_name();
}
