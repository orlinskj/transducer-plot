#ifndef FUNCTIONFORM_H
#define FUNCTIONFORM_H

#include <QWidget>
#include <QChart>
#include <QAbstractItemView>
#include "viewmodel/treemodel/treeitemmodel.h"
#include "viewmodel/functionitem.h"
#include "view/plotpresenter.h"

#include <memory>
#include <functional>

using namespace QtCharts;

namespace Ui {
class FunctionForm;
}

class FunctionForm : public QWidget
{
    Q_OBJECT

public:
    FunctionForm(
            QWidget *parent,
            QAbstractItemView* transduder_view,
            QAbstractItemView* plot_view,
            PlotPresenter* presenter,
            const std::function<void(PlotItem*)>& add_callback,
            const std::function<void(void)>& cancel_callback);
    ~FunctionForm();

protected:
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void select_default();

protected slots:
    void update_set_list(const QModelIndex& index);
    void list_clicked(const QModelIndex& index);
    void add_function();
    void update_widgets();

private:
    Ui::FunctionForm *ui_;

    QChart* preview_;
    std::unique_ptr<Function> preview_function_;
    std::unique_ptr<FunctionItem> preview_function_item_;

    TreeItemModel sets_model_;

    QAbstractItemView* plot_view_;
    QAbstractItemView* transducer_view_;
    PlotPresenter* presenter_;

    std::function<void(PlotItem*)> add_callback_;
    std::function<void(void)> cancel_callback_;

    QModelIndex domain_index_;
    QModelIndex codomain_index_;

};

#endif // FUNCTIONFORM_H
