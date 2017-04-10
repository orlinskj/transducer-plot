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

