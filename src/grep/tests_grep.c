#include <check.h>
#include <stdio.h>

START_TEST(test_my_grep) {
  FILE *my;
  FILE *orig;

  my = fopen("my_grep_test.txt", "rt");
  orig = fopen("orig_grep_test.txt", "rt");

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
  Suite *st = suite_create("Grep");
  SRunner *sr = srunner_create(st);
  int nf;

  TCase *my_grep_case = tcase_create("my_grep_test");
  suite_add_tcase(st, my_grep_case);
  tcase_add_test(my_grep_case, test_my_grep);

  srunner_run_all(sr, CK_VERBOSE);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}
