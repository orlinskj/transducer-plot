#define BOOST_TEST_MODULE transducer_plot
#include <boost/test/included/unit_test.hpp>

#include "../src/model/plotstoreitemmodel.h"

BOOST_AUTO_TEST_CASE( plot_store_item_model_index )
{
    ac::PlotStoreItemModel model;

    QModelIndex index = model.index(0,0,QModelIndex());
    BOOST_CHECK(!index.isValid());

    index = model.index(0,1,QModelIndex());
    BOOST_CHECK(!index.isValid());

    auto plot = model.add_plot(new ac::Plot("Wykres"));

    index = model.index(0,0,QModelIndex());
    BOOST_CHECK( index.isValid() );

    index = model.index(0,1,QModelIndex());
    BOOST_CHECK(!index.isValid());
}

BOOST_AUTO_TEST_CASE( plot_store_item_model_add_plot )
{
    ac::PlotStoreItemModel model;

    constexpr int PLOTS_COUNT = 10;

    std::vector<Plot*> plots;
    for (int i=0; i<PLOTS_COUNT; i++)
    {
        std::string s { "01234567890", size_t(i), 1 };
        Plot* p = model.add_plot(new ac::Plot());
        p->set_name(s);
        plots.push_back(p);
    }


    for (int i=0; i<PLOTS_COUNT; i++)
    {
        auto index = model.index(i,0,QModelIndex());
        BOOST_CHECK( index.isValid() );

        std::string s { "01234567890", size_t(i), 1 };

        std::string p1s = model.data(index,Qt::DisplayRole).value<QString>().toStdString();
        BOOST_CHECK( p1s == s );
    }

    BOOST_CHECK( model.rowCount() == PLOTS_COUNT );
}

BOOST_AUTO_TEST_CASE( plot_add_function )
{
    ac::PlotStoreItemModel model;

    auto plot = model.add_plot(new ac::Plot());
    plot->add_function(new ac::Function());

    auto parent = model.index(0,0,QModelIndex());
    auto func_index = model.index(0,0,parent);

    BOOST_CHECK(model.rowCount(parent) == 1);
    BOOST_CHECK(func_index.isValid());
    BOOST_CHECK(model.data(func_index,ac::PlotStoreItem::Role).canConvert<Function*>());
}

BOOST_AUTO_TEST_CASE(plot_row_count_column_count)
{
    ac::PlotStoreItemModel model;

    BOOST_CHECK(model.rowCount() == 0);
    BOOST_CHECK(model.columnCount() == 0);

    auto plot = model.add_plot(new ac::Plot());

    BOOST_CHECK(model.rowCount() == 1);
    BOOST_CHECK(model.columnCount() == 1);

    BOOST_CHECK(model.rowCount(model.index(plot)) == 0);

    auto f1 = model.add_function(plot, new ac::Function());
    auto f2 = model.add_function(plot, new ac::Function());

    BOOST_CHECK(model.rowCount(model.index(plot)) == 2);
    BOOST_CHECK(model.columnCount(model.index(plot)) == 1);

    BOOST_CHECK(model.rowCount() == 1);
    BOOST_CHECK(model.columnCount() == 1);

    BOOST_CHECK(model.rowCount(model.index(f1)) == 0);
    BOOST_CHECK(model.columnCount(model.index(f1)) == 0);
}
