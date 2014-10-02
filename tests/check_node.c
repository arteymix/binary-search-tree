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

Suite *node_suite(void)
{
    TCase *core_tc = tcase_create("core");
    tcase_add_test(core_tc, test_node_new);
    tcase_add_test(core_tc, test_node_search);
    tcase_add_test(core_tc, test_node_insert);
    tcase_add_test(core_tc, test_node_delete);

    TCase *definition_tc = tcase_create("definition");
    tcase_add_test(core_tc, test_node_definition);

    Suite *s = suite_create("node");
    suite_add_tcase(s, core_tc);
    suite_add_tcase(s, definition_tc);

    return s;
}

int main(void)
{
    Suite *s = node_suite();
    SRunner *sr = srunner_create(s);
 
    srunner_run_all(sr, CK_NORMAL);

    srunner_free(sr);

    return EXIT_SUCCESS;
}
