#include <check.h>
#include <stdio.h>

START_TEST(test_my_cat) {
    FILE *my;
    FILE *orig;

    my = fopen("my_test.txt", "rt");
    orig = fopen("orig_test.txt", "rt");

    int c = 0, o = 0;

    while (c != EOF || o != EOF) {
        c = getc(my);
        o = getc(orig);
        ck_assert_int_eq(c, o);
    }
    fclose(my);
    fclose(orig);
}
END_TEST

int main() {
    Suite *st = suite_create("Cat");
    SRunner *sr = srunner_create(st);
    int nf;

    TCase *my_cat_case = tcase_create("my_cat_test");
    suite_add_tcase(st, my_cat_case);
    tcase_add_test(my_cat_case, test_my_cat);

    srunner_run_all(sr, CK_VERBOSE);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
