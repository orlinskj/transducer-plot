#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../src/model/function.h"
#include "../src/model/set.h"
#include "../src/model/unit.h"

#include <vector>
#include <cmath>

BOOST_AUTO_TEST_CASE(function_value_at)
{
    std::vector<SetType> dv, cv;

    for(size_t i=0; i<1000; i++)
    {
        dv.push_back( 100.0+0.4*double(i) );
        cv.push_back( double(i) + 5.0*std::sin(i/4.0) );
    }

    Set d(std::move(dv), Unit("unit1","U1","p"));
    Set c(std::move(cv), Unit("unit2","U2","q"));

    Function f(nullptr, &d, &c);

    double eps = 1e-8;

    boost::optional<SetType> oi;

    oi = f.value_at(100.0);
    BOOST_CHECK(oi);
    if (oi)
        BOOST_CHECK_CLOSE(*oi, 0, eps);
}

BOOST_AUTO_TEST_CASE(function_value_at2)
{
    std::vector<SetType> dv, cv;

    for(size_t i=0; i<5; i++)
    {
        dv.push_back( i );
        cv.push_back( i );
    }

    Set d(std::move(dv), Unit("unit1","U1","p"));
    Set c(std::move(cv), Unit("unit2","U2","q"));

    Function f(nullptr, &d, &c);

    double eps = 1e-8;

    boost::optional<SetType> oi;

    oi = f.value_at(-4.0);
    BOOST_CHECK(!oi);

    oi = f.value_at(0.0);
    BOOST_CHECK(oi);
    BOOST_CHECK_CLOSE(*oi, 0, eps);

    oi = f.value_at(2.01);
    BOOST_CHECK(oi);
    BOOST_CHECK_CLOSE(*oi, 2.01, eps);

    oi = f.value_at(2.5);
    BOOST_CHECK(oi);
    BOOST_CHECK_CLOSE(*oi, 2.5, eps);

    oi = f.value_at(2.99);
    BOOST_CHECK(oi);
    BOOST_CHECK_CLOSE(*oi, 2.99, eps);

    oi = f.value_at(4.0);
    BOOST_CHECK(oi);
    BOOST_CHECK_CLOSE(*oi, 4.0, eps);

    oi = f.value_at(4.1);
    BOOST_CHECK(!oi);
}

