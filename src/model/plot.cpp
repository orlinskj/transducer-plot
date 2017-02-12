#include "plot.h"

#include <algorithm>
#include <sstream>
#include "plotstoreitemmodel.h"

Plot::Plot() : name_("Wykres") {}
Plot::Plot(const std::string& name) : name_(name) { }

Plot::~Plot() {}

const std::string& Plot::name() const
{
    return name_;
}

void Plot::set_name(const std::string& name)
{
    name_ = name;
}

std::string Plot::description() const
{
    std::stringstream ss;
    ss << "Wykres";
    if (children_count())
    {
        for(int i=0; i<children_count(); i++)
        {
            auto func = dynamic_cast<Function*>(child(i));
            ss << " " << func->codomain()->unit().name();
            if (i < children_count()-1)
                ss << ",";
        }

        auto func = dynamic_cast<Function*>(child(0));
        ss << " od " << func->domain()->unit().name();
    }

    return ss.str();
}

std::string Plot::to_string() const
{
    return name();
}
