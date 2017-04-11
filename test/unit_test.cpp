#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../src/model/unit.h"

#include <vector>
#include <cmath>

BOOST_AUTO_TEST_CASE(combined_unit)
{
    CombinedUnit unit;
    Unit a("name a","symbol a","unit a");
    Unit b("name b","symbol b","unit b");
    Unit a2("name a2","symbol a2","unit a");

    BOOST_CHECK(unit.match_unit(a));
    BOOST_CHECK(unit.match_unit(b));
    BOOST_CHECK(unit.match_unit(a2));

    unit.add_unit(a);

    BOOST_CHECK(!unit.match_unit(b));
    BOOST_CHECK(unit.match_unit(a2));

    BOOST_CHECK_EQUAL(unit.name(), "name a");
    BOOST_CHECK_EQUAL(unit.symbol(), "symbol a");
    BOOST_CHECK_EQUAL(unit.unit(), "unit a");

    unit.add_unit(a2);

    BOOST_CHECK(!unit.match_unit(b));

    BOOST_CHECK_EQUAL(unit.name(), "name a, name a2");
    BOOST_CHECK_EQUAL(unit.symbol(), "symbol a, symbol a2");
    BOOST_CHECK_EQUAL(unit.unit(), "unit a");

    unit.remove_unit(a);

    BOOST_CHECK_EQUAL(unit.name(), "name a2");
    BOOST_CHECK_EQUAL(unit.symbol(), "symbol a2");
    BOOST_CHECK_EQUAL(unit.unit(), "unit a");

}

BOOST_AUTO_TEST_CASE(unit_representation)
{
    Unit c("capacity","C","F");
    Unit i("inductance","L","H");

    BOOST_CHECK_EQUAL(c.nice_repr<double>(12.345,5), "12.345 F");
    BOOST_CHECK_EQUAL(c.nice_repr<double>(123.45,5), "123.45 F");
    BOOST_CHECK_EQUAL(c.nice_repr<double>(1234.5,5), "1.2345 kF");

    BOOST_CHECK_EQUAL(c.nice_repr<double>(1e-3,5), "1 mF");
    BOOST_CHECK_EQUAL(c.nice_repr<double>(1.23e-6,5), "1.23 µF");
    BOOST_CHECK_EQUAL(c.nice_repr<double>(1.23e-8,5), "12.3 nF");

    double eps = 1e-13;

    BOOST_CHECK_CLOSE( c.value_from_repr<double>("1.23 nF"), 1.23e-9, eps);
    BOOST_CHECK_CLOSE( c.value_from_repr<double>("1.23nF"), 1.23e-9, eps);

    BOOST_CHECK_CLOSE( c.value_from_repr<double>("1.23 F"), 1.23, eps);
    BOOST_CHECK_CLOSE( c.value_from_repr<double>("1.23F"), 1.23, eps);

    BOOST_CHECK_CLOSE( c.value_from_repr<double>("1.23 µF"), 1.23e-6, eps);
    BOOST_CHECK_CLOSE( c.value_from_repr<double>("1.23µF"), 1.23e-6, eps);

}

