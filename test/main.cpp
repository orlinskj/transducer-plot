#define BOOST_TEST_MODULE transducer_plot
#include <boost/test/included/unit_test.hpp>

#include "../src/model/treemodel/treeitemmodel.h"
#include "../src/model/treemodel/treeitem.h"
#include "../src/model/treemodel/flattentreeproxymodel.h"

struct F{
    F(){
        t1 = tree.append(new TreeItem);
        t2 = tree.append(new TreeItem);
        t3 = tree.append(new TreeItem);

        t11 = t1->append(new TreeItem);
        t12 = t1->append(new TreeItem);

        t111 = t11->append(new TreeItem);
        t112 = t11->append(new TreeItem);
        t113 = t11->append(new TreeItem);

        t121 = t12->append(new TreeItem);
        t122 = t12->append(new TreeItem);

        t31 = t3->append(new TreeItem);
        t32 = t3->append(new TreeItem);

        t311 = t31->append(new TreeItem);
        t312 = t31->append(new TreeItem);

        t3111 = t311->append(new TreeItem);
        t3112 = t311->append(new TreeItem);

        t3121 = t312->append(new TreeItem);
        t3122 = t312->append(new TreeItem);

        t321 = t32->append(new TreeItem);
        t322 = t32->append(new TreeItem);

        t3211 = t321->append(new TreeItem);
        t3212 = t321->append(new TreeItem);

        t3221 = t322->append(new TreeItem);
        t3222 = t322->append(new TreeItem);
    }

    TreeItemModel tree;
        TreeItem* t1;
        TreeItem* t11;
            TreeItem* t111;
            TreeItem* t112;
            TreeItem* t113;
        TreeItem* t12;
            TreeItem* t121;
            TreeItem* t122;
        TreeItem* t2;
        TreeItem* t3;
            TreeItem* t31;
                TreeItem* t311;
                    TreeItem* t3111;
                    TreeItem* t3112;
                TreeItem* t312;
                    TreeItem* t3121;
                    TreeItem* t3122;
            TreeItem* t32;
                TreeItem* t321;
                    TreeItem* t3211;
                    TreeItem* t3212;
                TreeItem* t322;
                    TreeItem* t3221;
                    TreeItem* t3222;
};
/*
virtual TreeItem* child(int index) const;
virtual TreeItem* parent() const;
virtual const TreeItem* absolute_child(int index) const;
virtual void path_to_root(std::vector<TreeItem*>* path) const;

virtual void remove(TreeItem* item);
virtual TreeItem* append(TreeItem* item);
virtual void kill();
virtual void kill_children();
*/

BOOST_FIXTURE_TEST_CASE( children_count, F)
{
    BOOST_CHECK_EQUAL(tree.children_count(), 3);
    BOOST_CHECK_EQUAL(t1->children_count(), 2);
    BOOST_CHECK_EQUAL(t11->children_count(), 3);
    BOOST_CHECK_EQUAL(t111->children_count(), 0);
    BOOST_CHECK_EQUAL(t112->children_count(), 0);
    BOOST_CHECK_EQUAL(t113->children_count(), 0);

    BOOST_CHECK_EQUAL(t12->children_count(), 2);
    BOOST_CHECK_EQUAL(t121->children_count(), 0);
    BOOST_CHECK_EQUAL(t122->children_count(), 0);

    BOOST_CHECK_EQUAL(t2->children_count(), 0);
    BOOST_CHECK_EQUAL(t3->children_count(), 2);

    BOOST_CHECK_EQUAL(t31->children_count(), 2);
    BOOST_CHECK_EQUAL(t32->children_count(), 2);
    BOOST_CHECK_EQUAL(t311->children_count(), 2);
    BOOST_CHECK_EQUAL(t312->children_count(), 2);
    BOOST_CHECK_EQUAL(t321->children_count(), 2);
    BOOST_CHECK_EQUAL(t322->children_count(), 2);

    BOOST_CHECK_EQUAL(t3111->children_count(), 0);
    BOOST_CHECK_EQUAL(t3112->children_count(), 0);
    BOOST_CHECK_EQUAL(t3121->children_count(), 0);
    BOOST_CHECK_EQUAL(t3122->children_count(), 0);

    BOOST_CHECK_EQUAL(t3211->children_count(), 0);
    BOOST_CHECK_EQUAL(t3212->children_count(), 0);
    BOOST_CHECK_EQUAL(t3221->children_count(), 0);
    BOOST_CHECK_EQUAL(t3222->children_count(), 0);
}

BOOST_FIXTURE_TEST_CASE( ancestor_count, F)
{
    BOOST_CHECK_EQUAL(tree.ancestor_count(), 24);
    BOOST_CHECK_EQUAL(t1->ancestor_count(), 7);
    BOOST_CHECK_EQUAL(t11->ancestor_count(), 3);
    BOOST_CHECK_EQUAL(t111->ancestor_count(), 0);
    BOOST_CHECK_EQUAL(t112->ancestor_count(), 0);
    BOOST_CHECK_EQUAL(t113->ancestor_count(), 0);

    BOOST_CHECK_EQUAL(t12->ancestor_count(), 2);
    BOOST_CHECK_EQUAL(t121->ancestor_count(), 0);
    BOOST_CHECK_EQUAL(t122->ancestor_count(), 0);

    BOOST_CHECK_EQUAL(t2->ancestor_count(), 0);
    BOOST_CHECK_EQUAL(t3->ancestor_count(), 14);

    BOOST_CHECK_EQUAL(t31->ancestor_count(), 6);
    BOOST_CHECK_EQUAL(t32->ancestor_count(), 6);

    BOOST_CHECK_EQUAL(t311->ancestor_count(), 2);
    BOOST_CHECK_EQUAL(t312->ancestor_count(), 2);

    BOOST_CHECK_EQUAL(t321->ancestor_count(), 2);
    BOOST_CHECK_EQUAL(t322->ancestor_count(), 2);
}

BOOST_FIXTURE_TEST_CASE( child_index, F )
{
    BOOST_CHECK_EQUAL(tree.child_index(t1), 0);
    BOOST_CHECK_EQUAL(tree.child_index(t2), 1);
    BOOST_CHECK_EQUAL(tree.child_index(t3), 2);

    BOOST_CHECK_EQUAL(t1->child_index(t11), 0);
    BOOST_CHECK_EQUAL(t1->child_index(t12), 1);

    BOOST_CHECK_EQUAL(t11->child_index(t111), 0);
    BOOST_CHECK_EQUAL(t11->child_index(t112), 1);
    BOOST_CHECK_EQUAL(t11->child_index(t113), 2);

    BOOST_CHECK_EQUAL(t12->child_index(t121), 0);
    BOOST_CHECK_EQUAL(t12->child_index(t122), 1);
}

BOOST_FIXTURE_TEST_CASE( child_index_negative, F)
{
    BOOST_CHECK_LT(tree.child_index(t111), 0);
    BOOST_CHECK_LT(tree.child_index(t112), 0);
    BOOST_CHECK_LT(tree.child_index(t113), 0);

    BOOST_CHECK_LT(tree.child_index(t11), 0);
    BOOST_CHECK_LT(tree.child_index(t12), 0);

    BOOST_CHECK_LT(t1->child_index(t3), 0);
    BOOST_CHECK_LT(t1->child_index(&tree), 0);
}

BOOST_FIXTURE_TEST_CASE( index_tc, F )
{
    BOOST_CHECK_LT(tree.TreeItem::index(), 0);

    BOOST_CHECK_EQUAL(t1->index(), 0);
    BOOST_CHECK_EQUAL(t2->index(), 1);
    BOOST_CHECK_EQUAL(t3->index(), 2);

    BOOST_CHECK_EQUAL(t11->index(), 0);
    BOOST_CHECK_EQUAL(t12->index(), 1);

    BOOST_CHECK_EQUAL(t111->index(), 0);
    BOOST_CHECK_EQUAL(t112->index(), 1);
    BOOST_CHECK_EQUAL(t113->index(), 2);

    BOOST_CHECK_EQUAL(t121->index(), 0);
    BOOST_CHECK_EQUAL(t122->index(), 1);
}

BOOST_FIXTURE_TEST_CASE( absolute_index, F )
{
    BOOST_CHECK_EQUAL( tree.absolute_index(), 0);

    BOOST_CHECK_EQUAL( t1->absolute_index(), 1);
    BOOST_CHECK_EQUAL( t2->absolute_index(), 9);
    BOOST_CHECK_EQUAL( t3->absolute_index(), 10);

    BOOST_CHECK_EQUAL( t11->absolute_index(), 2);
    BOOST_CHECK_EQUAL( t12->absolute_index(), 6);

    BOOST_CHECK_EQUAL( t111->absolute_index(), 3);
    BOOST_CHECK_EQUAL( t112->absolute_index(), 4);
    BOOST_CHECK_EQUAL( t113->absolute_index(), 5);

    BOOST_CHECK_EQUAL( t121->absolute_index(), 7);
    BOOST_CHECK_EQUAL( t122->absolute_index(), 8);

    BOOST_CHECK_EQUAL( t31->absolute_index(), 11);
    BOOST_CHECK_EQUAL( t32->absolute_index(), 18);

    BOOST_CHECK_EQUAL( t311->absolute_index(), 12);
    BOOST_CHECK_EQUAL( t312->absolute_index(), 15);

    BOOST_CHECK_EQUAL( t321->absolute_index(), 19);
    BOOST_CHECK_EQUAL( t322->absolute_index(), 22);

    BOOST_CHECK_EQUAL( t3111->absolute_index(), 13);
    BOOST_CHECK_EQUAL( t3112->absolute_index(), 14);

    BOOST_CHECK_EQUAL( t3121->absolute_index(), 16);
    BOOST_CHECK_EQUAL( t3122->absolute_index(), 17);

    BOOST_CHECK_EQUAL( t3211->absolute_index(), 20);
    BOOST_CHECK_EQUAL( t3212->absolute_index(), 21);

    BOOST_CHECK_EQUAL( t3221->absolute_index(), 23);
    BOOST_CHECK_EQUAL( t3222->absolute_index(), 24);
}

BOOST_FIXTURE_TEST_CASE( tree_absolute_child, F )
{

    BOOST_CHECK_EQUAL( tree.absolute_child(0)->absolute_index(), 0);
    BOOST_CHECK_EQUAL( tree.absolute_child(1)->absolute_index(), 1);
    BOOST_CHECK_EQUAL( tree.absolute_child(9)->absolute_index(), 9);
    BOOST_CHECK_EQUAL( tree.absolute_child(10)->absolute_index(), 10);

    BOOST_CHECK( tree.absolute_child(0) == &tree);
    BOOST_CHECK( tree.absolute_child(1) == t1);
    BOOST_CHECK( tree.absolute_child(9) == t2);
    BOOST_CHECK( tree.absolute_child(10) == t3);

    BOOST_CHECK( t1->absolute_child(0) == t1);
    BOOST_CHECK( t1->absolute_child(1) == t11);
    BOOST_CHECK( t1->absolute_child(2) == t111);
    BOOST_CHECK( t1->absolute_child(3) == t112);
    BOOST_CHECK( t1->absolute_child(4) == t113);

    BOOST_CHECK( t1->absolute_child(5) == t12);

}

BOOST_FIXTURE_TEST_CASE( flattentreeproxymodel, F )
{
    FlattenTreeProxyModel proxy;
    proxy.setSourceModel(&tree);

    BOOST_CHECK_EQUAL( proxy.rowCount(QModelIndex()), tree.ancestor_count());

    for (int i=0; i<proxy.rowCount(QModelIndex()); i++)
    {
        BOOST_CHECK_EQUAL(proxy.rowCount(proxy.index(i,0,QModelIndex())), 0);
    }

    auto item = static_cast<TreeItem*>(proxy.index(0,0,QModelIndex()).internalPointer());
    BOOST_CHECK_EQUAL(item, t1);

    item = static_cast<TreeItem*>(proxy.index(1,0,QModelIndex()).internalPointer());
    BOOST_CHECK_EQUAL(item, t11);
}
