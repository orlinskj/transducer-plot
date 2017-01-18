#include "plot.h"

#include <algorithm>
#include "plotstoreitemmodel.h"

ac::Plot::Plot() : name_("Wykres") {}
ac::Plot::Plot(const std::string& name) : name_(name) { }

ac::Plot::~Plot()
{
    for (auto it=functions_.cbegin(); it!=functions_.cend(); it++)
        delete *it;
}

Function* ac::Plot::add_function(Function *f)
{
    f->set_plot(this);
    functions_.push_back(f);
    return functions_.back();
}

bool ac::Plot::remove_function(const ac::Function* f)
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

bool ac::Plot::find_function(const Function *f) const
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

void ac::Plot::set_name(const std::string& name)
{
    name_ = name;
}

int ac::Plot::function_count() const
{
    return functions_.size();
}

const ac::Function* ac::Plot::function_at(int index) const
{
    return functions_.at(index);
}

std::string ac::Plot::to_string() const
{
    return get_name();
}

int ac::Plot::function_row(const Function *f) const
{
    for (auto it=functions_.cbegin(); it!=functions_.cend(); it++)
    {
        if (*it == f)
            return int(it-functions_.cbegin());
    }

    return -1;
}
