#ifndef MODEL_PLOT_H
#define MODEL_PLOT_H

#include <vector>
#include <memory>

#include "function.h"
#include "plotstoreitem.h"

namespace ac
{

class Function;
class Plot;

using Plot_ptr = std::shared_ptr<Plot>;

// Representation of plot.
// One plot can have multiple functions binded to it.
// Attached function is displayed on plot.
class Plot : public PlotStoreItem
{
public:
    static Plot_ptr make_ptr(Plot&& plot);

    Plot();
    Plot(const std::string& name);
    virtual void add_function(const Function& f);
    virtual bool remove_function(const Function& f);
    virtual bool find_function(const Function& f) const;

    int function_count() const;
    const Function& function_at(int index) const;

    void set_name(const std::string& name);
    const std::string& get_name() const;

    std::string to_string() const override;

protected:
    std::vector<Function> functions_;
    std::string name_;
};

}

Q_DECLARE_METATYPE(ac::Plot_ptr)


#endif // PLOT_H
