#include "functionform.h"
#include "ui_functionform.h"

#include <QItemSelectionModel>
#include <QAbstractItemView>
#include <QTextItem>
#include <QStackedWidget>

#include "viewmodel/transduceritem.h"
#include "viewmodel/setitem.h"
#include "viewmodel/plotitem.h"
#include "viewmodel/functionitem.h"
#include "view/setitemdelegate.h"
#include "view/plotpresenter.h"

FunctionForm::FunctionForm(
        QWidget *parent,
        QAbstractItemView* transduder_view,
        QAbstractItemView *plot_view,
        PlotPresenter* presenter) :
    QWidget(parent),
    ui_(new Ui::FunctionForm),
    preview_(new QChart),
    plot_view_(plot_view),
    transducer_view_(transduder_view),
    presenter_(presenter)
{
    ui_->setupUi(this);

    // init view
    ui_->listView->setModel(&sets_model_);
    ui_->listView->setItemDelegate(new SetItemDelegate(domain_index_,codomain_index_));

    ui_->graphicsView->setScene(new QGraphicsScene);
    ui_->graphicsView->scene()->addItem(preview_);
    preview_->resize(ui_->graphicsView->size());
    preview_->legend()->hide();

    ui_->graphicsView->setContentsMargins(0,0,0,0);
    preview_->setMargins(QMargins(10,10,10,10));
    preview_->setContentsMargins(0,0,0,0);

    ui_->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui_->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui_->graphicsView->setContentsMargins(0,0,0,0);
    auto preview_text = new QGraphicsTextItem(tr("(Podgląd)"));
    ui_->graphicsView->scene()->addItem(preview_text);
    preview_text->setPos(2,2);
    preview_text->setDefaultTextColor(QPalette().mid().color());

    // initialization
    update_set_list(this->ui_->listView->currentIndex());
    update_widgets();

    // signals
    connect(transduder_view->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &FunctionForm::update_set_list);

    connect(ui_->listView, &QListView::clicked,
            this, &FunctionForm::list_clicked);

    connect(plot_view_->selectionModel(), &QItemSelectionModel::currentChanged,
            this, [this](const QModelIndex&){ update_widgets(); });

    connect(ui_->addButton, &QPushButton::clicked,
            this, &FunctionForm::add_function);
}

FunctionForm::~FunctionForm()
{
    delete ui_;
}

void FunctionForm::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    preview_->resize(ui_->graphicsView->size());
}

void FunctionForm::update_set_list(const QModelIndex& index)
{
    preview_->removeAllSeries();
    preview_function_.release();
    preview_function_item_.release();

    sets_model_.kill_children();

    domain_index_ = QModelIndex();
    codomain_index_ = QModelIndex();

    if (index.isValid())
    {
        auto item = static_cast<TreeItem*>(index.internalPointer());
        auto transducer_item = dynamic_cast<TransducerItem*>(item);

        for (int i=0; i<transducer_item->children_count(); i++){
            auto set = dynamic_cast<SetItem*>(transducer_item->child(i));
            sets_model_.append(new SetItem((*set)()));
        }
    }
}

void FunctionForm::add_function()
{
    auto domain = dynamic_cast<SetItem*>(domain_index_.data(TreeItem::Role).value<TreeItem*>())->value();
    auto codomain = dynamic_cast<SetItem*>(codomain_index_.data(TreeItem::Role).value<TreeItem*>())->value();
    auto plot_item = dynamic_cast<PlotItem*>(plot_view_->currentIndex().data(TreeItem::Role).value<TreeItem*>());
    auto transducer = dynamic_cast<TransducerItem*>(transducer_view_->currentIndex().data(TreeItem::Role).value<TreeItem*>())->value();

    plot_item->append(new FunctionItem(new Function(transducer,domain,codomain)));

    auto parent = dynamic_cast<QStackedWidget*>(this->parentWidget());

    if (!parent){
        parent = dynamic_cast<QStackedWidget*>(this->parentWidget()->parentWidget());
    }

    if (parent){
        parent->setCurrentIndex(0);
        presenter_->show_plot(plot_item);
    }
}

void FunctionForm::update_widgets()
{
    bool enabled = domain_index_.isValid() && codomain_index_.isValid() && plot_view_->currentIndex().isValid();
    ui_->addButton->setEnabled(enabled);

    if (domain_index_.isValid() && codomain_index_.isValid()){
        auto domain = dynamic_cast<SetItem*>(domain_index_.data(TreeItem::Role).value<TreeItem*>())->value();
        auto codomain = dynamic_cast<SetItem*>(codomain_index_.data(TreeItem::Role).value<TreeItem*>())->value();

        if (!preview_function_ || preview_function_->domain() != domain || preview_function_->codomain() != codomain){
            preview_function_item_.release();
            preview_function_.reset(new Function(nullptr, domain, codomain));
            preview_function_item_.reset(new FunctionItem(preview_function_.get()));
            preview_->removeAllSeries();
            preview_->addSeries(preview_function_item_->series());
        }
    }
    else{
        preview_->removeAllSeries();
        preview_function_.release();
        preview_function_item_.release();
    }
}

void FunctionForm::list_clicked(const QModelIndex &index)
{
    if (index.isValid()){
        if (codomain_index_ == index){
            codomain_index_ = QModelIndex();
        }
        else if (domain_index_ == index){
            domain_index_ = QModelIndex();
        }
        else if(!domain_index_.isValid()){
            domain_index_ = index;
        }
        else{
            codomain_index_ = index;
        }

        //ui_->listView->update(index);
        auto current_index = ui_->listView->currentIndex();
        ui_->listView->reset();
        ui_->listView->setCurrentIndex(current_index);
    }

    update_widgets();
}

void FunctionForm::select_default()
{
    if (!plot_view_->selectionModel()->hasSelection()){
        if (plot_view_->model()->rowCount() > 0){
            plot_view_->setCurrentIndex(plot_view_->model()->index(0,0));
        }
    }

    if (!transducer_view_->selectionModel()->hasSelection()){
        if (transducer_view_->model()->rowCount() > 0){
            transducer_view_->setCurrentIndex(transducer_view_->model()->index(0,0));
        }
    }

    domain_index_ = QModelIndex();
    codomain_index_ = QModelIndex();
    ui_->listView->reset();
    preview_->removeAllSeries();
}
