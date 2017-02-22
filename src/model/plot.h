#ifndef MODEL_PLOT_H
#define MODEL_PLOT_H

#include <vector>
#include <memory>

#include "function.h"

class Function;

// Representation of plot.
// One plot can have multiple functions binded to it.
// Binded functions are displayed on the plot.
class Plot
{
public:
    Plot(const std::string& name = "Wykres");

    void set_name(const std::string& name);
    const std::string& name() const;
    std::string description() const;

    void add_function(Function* f);
    void remove_function(Function* f);

    // required by TreeItemT
    operator std::string() const;

protected:
    std::vector<Function*> functions_;
    std::string name_;
};


#endif // PLOT_H
