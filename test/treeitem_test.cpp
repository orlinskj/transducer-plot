#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../src/viewmodel/treemodel/treeitemmodel.h"
#include "../src/viewmodel/transduceritem.h"

#include <vector>
#include <cmath>

BOOST_AUTO_TEST_CASE(treeitemmodel_ctor)
{
    TreeItemModel model;
    BOOST_CHECK(static_cast<TreeItem&>(model).parent() == nullptr);
    BOOST_CHECK_EQUAL(model.children_count(), 0);
}

struct Custom {
    int x;
    operator std::string(){
        return std::string("some string");
    }
    bool operator == (const Custom& c) const{
        return x == c.x;
    }
};

std::ostream& operator<<(std::ostream& s, const Custom& c)
{
    return s << c.x;
}

BOOST_AUTO_TEST_CASE(treeitemmodel_append)
{
    TreeItemModel model;

    Custom c;

    using T1 = TreeItemTOwner<std::string>;
    using T2 = TreeItemT<Custom>;

    auto t1 = model.append(new T1(new std::string("Hello")));
    auto t2 = model.append(new T2(&c));

    BOOST_CHECK_EQUAL(model.children_count(), 2);
    BOOST_CHECK_EQUAL(t1->children_count(), 0);
    BOOST_CHECK_EQUAL(t2->children_count(), 0);

    BOOST_CHECK(t1->parent() == &model);
    BOOST_CHECK(t2->parent() == &model);

    BOOST_CHECK_EQUAL(*(dynamic_cast<T1*>(t1)->value()), std::string("Hello"));
    BOOST_CHECK_EQUAL(dynamic_cast<T2*>(t2)->value(), &c);
    BOOST_CHECK_EQUAL(*(dynamic_cast<T2*>(t2)->value()), c);
}
