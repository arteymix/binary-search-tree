#include <check.h>
#include "../src/bst.c"

START_TEST(bst_insert)
{
    node *root = malloc(sizeof(node));
    node *l = malloc(sizeof(node));
    node *r = malloc(sizeof(node));

    root->term = "b";
    l->term = "a";
    r->term = "c";

    node_insert(root, l);
    node_insert(root, r);

    ck_assert(root->left == l);
    ck_assert(root->right == r);
}
END_TEST

START_TEST (bst_search)
{
    node *root = malloc(sizeof(node));
    node *l = malloc(sizeof(node));
    node *r = malloc(sizeof(node));

    root->term = "b";
    l->term = "a";
    r->term = "c";

    node_insert(root, l);
    node_insert(root, r);

    ck_assert(root->left == l);
    ck_assert(root->right == r);
}
END_TEST
