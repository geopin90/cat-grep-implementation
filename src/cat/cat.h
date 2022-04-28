#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <stdio.h>

struct struc {
    int flag_b;
    int flag_e;
    int flag_n;
    int flag_s;
    int flag_t;
    int flag_v;
};

void init(struct struc *fl);
void parser(int argnum, char **arglist, struct struc *fl);
void print_ch(FILE *f, struct struc *fl);

#endif  //  SRC_CAT_S21_CAT_H_
