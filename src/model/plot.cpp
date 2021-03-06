#include "plot.h"

#include <algorithm>
#include <sstream>

Plot::Plot(const std::string& name) : name_(name) { }

const std::string& Plot::name() const
{
    return name_;
}

void Plot::set_name(const std::string& name)
{
    name_ = name;
}

void Plot::add_function(Function *f)
{
    functions_.emplace_back(std::unique_ptr<Function>(f));
}

void Plot::remove_function(Function *f)
{
    auto it = std::find_if(functions_.begin(), functions_.end(), [f](const auto& func){
        return func.get() == f;
    });

    if (it != functions_.end())
        functions_.erase(it);
}

std::string Plot::description() const
{
    std::stringstream ss;
    ss << "Wykres";

    if (!functions_.size())
        return ss.str();

    for(auto it=functions_.begin(); it!=functions_.end(); ++it)
    {
        ss << " " << (*it)->codomain()->unit().name();
        if (it+1 != functions_.end())
            ss << ",";
        else
            ss << " od " << (*it)->domain()->unit().name();
    }

    return ss.str();
}

Plot::operator std::string() const
{
    return name();
}
