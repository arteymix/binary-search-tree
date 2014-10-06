#include <check.h>
#include "../src/node.c"

START_TEST(test_node_new)
{
    node *n = node_new("term", "definition");

    ck_assert_str_eq("term", n->term);
    ck_assert_str_eq("term", n->definition);
    ck_assert_ptr_eq(NULL, n->left);
    ck_assert_ptr_eq(NULL, n->right);

    free(n);
}
END_TEST

START_TEST(test_node_insert)
{
    node *root = node_new("b", "definition");
    node *l = node_new("a", "");
    node *r = node_new("c", "");

    node_insert(root, l);
    node_insert(root, r);

    ck_assert_ptr_eq(root->left, l);
    ck_assert_ptr_eq(root->right, r);

    free(root);
    free(l);
    free(r);
}
END_TEST

START_TEST(test_node_search)
{
    node *root = node_new("term", "definition");
}
END_TEST

START_TEST(test_node_delete)
{
    node *root = node_new("term", "definition");
}
END_TEST

START_TEST(test_node_definition)
{
    node *root = node_new("term", "definition");
}
END_TEST

int main(void)
{
    return EXIT_SUCCESS;
}
