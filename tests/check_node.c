#include <stdlib.h>
#include <check.h>

#include "../src/node.c"

START_TEST(test_node_new)
{
    node *n = node_new("term", "definition");

    ck_assert_str_eq("term", n->term);
    ck_assert_str_eq("definition", n->definition);
    ck_assert_ptr_eq(NULL, n->left);
    ck_assert_ptr_eq(NULL, n->right);

    node_free(n);
}
END_TEST

START_TEST(test_node_free)
{
    node *n = node_new("a", "a");
    node *l = node_new("b", "b");

    node_insert(n, l);

    ck_assert_ptr_eq(l, n->right);

    node_free(n);

    // @TODO vérifier que n et l sont désalloués
}
END_TEST

START_TEST(test_node_insert)
{
    node *root = node_new("b", "definition");
    node *l = node_new("a", "a");
    node *r = node_new("c", "c");

    node_insert(root, l);
    node_insert(root, r);

    ck_assert_ptr_eq(l, root->left);
    ck_assert_ptr_eq(r, root->right);

    ck_assert_str_eq("a", root->left->definition);
    ck_assert_str_eq("c", root->right->definition);

    node_free(root);
}
END_TEST

START_TEST(test_node_search)
{
    node *root = node_new("term", "definition");
    node *l = node_new("a", "");
    node *r = node_new("c", "");

    node_insert(root, l);
    node_insert(root, r);

    node *s = node_search(root, "a");

    ck_assert_ptr_eq(l, s);

    node_free(root);
}
END_TEST

START_TEST(test_node_delete)
{
    node *root = node_new("d", "definition");
    node *l = node_new("b", "b");
    node *ll = node_new("a", "a");
    node *lr = node_new("c", "c");

    node_insert(root, l);
    node_insert(root, ll);
    node_insert(root, lr);

    // ensure correct insertions
    ck_assert_ptr_eq(l, root->left);
    ck_assert_ptr_eq(ll, root->left->left);
    ck_assert_ptr_eq(lr, root->left->right);

    ck_assert_ptr_eq(l, node_delete(root, "b"));

    // ensure correct reinsertion
    ck_assert_ptr_eq(ll, root->left);
    ck_assert_ptr_eq(NULL, root->right);
    ck_assert_ptr_eq(NULL, root->left->left);
    ck_assert_ptr_eq(lr, root->left->right);

    // node is not found in the tree anymore
    ck_assert_ptr_eq(NULL, node_search(root, "b"));

    node_free(root);
}
END_TEST

START_TEST(test_node_definition_simple)
{
    node *root = node_new("simple", "haha");
    node *a = node_new("a", "b");
    node *b = node_new("b", "c");
    node *c = node_new("c", "c");

    node_insert(root, a);
    node_insert(root, b);
    node_insert(root, c);

    char *a_definition = node_definition(root, "a");
    char *b_definition = node_definition(root, "a");
    char *c_definition = node_definition(root, "a");

    ck_assert_str_eq("c", a_definition); // définition indirecte
    ck_assert_str_eq("c", b_definition); // définition directe
    ck_assert_str_eq("c", c_definition); // définition récursive

    node_free(root);
    free(a_definition);
    free(b_definition);
    free(c_definition);
}
END_TEST

START_TEST(test_node_definition_compound)
{
    node *root = node_new("compound", "a+c+b");

    node *a = node_new("a", "haha");
    node *b = node_new("b", "you");
    node *c = node_new("c", "lie");

    node_insert(root, a);
    node_insert(root, b);
    node_insert(root, c);

    char *definition = node_definition(root, "compound");

    ck_assert_str_eq("haha you lie", definition);

    node_free(root);
    free(definition);
}
END_TEST

Suite *node_suite(void)
{
    Suite *s = suite_create("node");
    TCase *tc_core = tcase_create("node");

    tcase_add_test(tc_core, test_node_new);
    tcase_add_test(tc_core, test_node_free);
    tcase_add_test(tc_core, test_node_insert);
    tcase_add_test(tc_core, test_node_delete);
    tcase_add_test(tc_core, test_node_search);
    tcase_add_test(tc_core, test_node_definition_simple);
    tcase_add_test(tc_core, test_node_definition_compound);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;

    Suite *s = node_suite();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
