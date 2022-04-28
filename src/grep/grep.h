#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <stdio.h>

struct flags {
    int flag_e;
    int flag_i;
    int flag_v;
    int flag_c;
    int flag_l;
    int flag_n;
    int flag_h;
    int flag_s;
    int flag_f;
    int temp_count;
    int struct_size;
    int file_count;
    int print_flag_l;
    char **pat_e;
};

void init(struct flags *fl);
void file_parse(int argnum, char **arglist, struct flags *fl);
void pat_init(struct flags *fl, int argnum);
void parser(int argnum, char **arglist, struct flags *fl);
char *e_strcpy(char *dest, const char *src);
void pat_realloc(struct flags *fl, int lsize);
void pat_destr(struct flags *fl, int argnum);
void flag_f(FILE *f, struct flags *fl);
void compare(int argnum, char **arglist, struct flags *fl);
void display(char *buf, struct flags *fl, char *argument, int *str_count, int f_str_count);

#endif  //  SRC_GREP_S21_GREP_H_
