#ifndef MODEL_PLOT_H
#define MODEL_PLOT_H

#include <vector>
#include <memory>

#include "function.h"

namespace ac
{

class Function;

using namespace std;

// Representation of plot.
// One plot can have multiple functions binded to it.
// Attached function is displayed on plot.
class Plot
{
public:
    static constexpr int Role = Qt::UserRole + 1;

    Plot();
    Plot(const std::string& name);
    virtual void add_function(const Function& f);
    virtual bool remove_function(const Function& f);
    virtual bool find_function(const Function& f) const;

    const std::string& get_name() const;

protected:
    std::vector<Function> functions_;
    std::string name_;
};

using Plot_ptr = std::shared_ptr<Plot>;

}

Q_DECLARE_METATYPE(ac::Plot_ptr)


#endif // PLOT_H
