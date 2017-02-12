#ifndef MODEL_PLOT_H
#define MODEL_PLOT_H

#include <vector>
#include <memory>

#include "function.h"
#include "treemodel/treeitem.h"

// namespace ac {

class Function;

// Representation of plot.
// One plot can have multiple functions binded to it.
// Binded functions are displayed on plot.
class Plot : public TreeItem
{
public:
    Plot();
    Plot(const std::string& name);
    ~Plot();

    void set_name(const std::string& name);
    const std::string& name() const;
    std::string description() const;

    std::string to_string() const override;

protected:
    // std::vector<Function*> functions_;
    std::string name_;
};

// } // namespace ac

Q_DECLARE_METATYPE(Plot*)


#endif // PLOT_H
