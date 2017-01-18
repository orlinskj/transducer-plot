#ifndef MODEL_PLOT_H
#define MODEL_PLOT_H

#include <vector>
#include <memory>

#include "function.h"
#include "treemodel/treenodeitem.h"

namespace ac
{

class Function;

// Representation of plot.
// One plot can have multiple functions binded to it.
// Binded functions are displayed on plot.
class Plot : public TreeNodeItem
{
public:
    Plot();
    Plot(const std::string& name);
    ~Plot();

    virtual Function* add_function(Function* f);
    virtual bool remove_function(const Function* f);
    virtual bool find_function(const Function* f) const;

    int function_count() const;
    const Function* function_at(int index) const;
    int function_row(const Function* f) const;

    void set_name(const std::string& name);
    const std::string& get_name() const;

    std::string to_string() const override;

protected:
    std::vector<Function*> functions_;
    std::string name_;
};

}

Q_DECLARE_METATYPE(ac::Plot*)


#endif // PLOT_H
