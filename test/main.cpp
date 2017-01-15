#define BOOST_TEST_MODULE transducer_plot
#include <boost/test/included/unit_test.hpp>

#include "../src/model/plotstoreitemmodel.h"

BOOST_AUTO_TEST_CASE( add_plot )
{
    ac::PlotStoreItemModel model;
    auto plot = model.add_plot();

    auto index = model.index(0,0,QModelIndex());
    BOOST_CHECK( index.isValid() );

    QVariant data = model.data(index,ac::PlotStoreItem::Role);
    BOOST_CHECK( data.canConvert<ac::Plot*>() );

    auto plot_ptr = data.value<ac::Plot*>();
    BOOST_CHECK( plot_ptr == plot);
}

BOOST_AUTO_TEST_CASE( add_plots_and_indexes )
{
    ac::PlotStoreItemModel model;

    constexpr int PLOTS_COUNT = 10;

    std::vector<Plot*> plots;
    for (int i=0; i<PLOTS_COUNT; i++)
    {
        std::string s { "01234567890", i, 1 };
        Plot* p = model.add_plot();
        p->set_name(s);
        plots.push_back(p);
    }


    for (int i=0; i<PLOTS_COUNT; i++)
    {
        auto index = model.index(i,0,QModelIndex());
        BOOST_CHECK( index.isValid() );

        std::string s { "01234567890", i, 1 };

        std::string p1s = model.data(index,Qt::DisplayRole).value<QString>().toStdString();
        BOOST_CHECK( p1s == s );
    }

    BOOST_CHECK( model.rowCount() == PLOTS_COUNT );
}
