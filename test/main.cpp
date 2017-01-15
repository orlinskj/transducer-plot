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
    BOOST_CHECK( data.canConvert<ac::Plot_ptr>() );

    auto plot_ptr = data.value<ac::Plot_ptr>();
    BOOST_CHECK( plot_ptr.get() == plot);

}
