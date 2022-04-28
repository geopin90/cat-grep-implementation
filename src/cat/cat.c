#include <stdio.h>
#include <string.h>
#include "cat.h"

int main(int argc, char **argv) {
    if (argc > 1) {
        struct struc fl;
        init(&fl);
        parser(argc, argv, &fl);
        FILE *f;
        for (int i = argc - 1; i > 0; i--) {
            if (argv[argc - i][0] != '-') {
                if ((f = fopen(argv[argc - i], "rt")) == NULL) {
                    printf("No such file or directory\n");
                } else {
                    print_ch(f, &fl);
                    fclose(f);
                }
            }
        }
    } else {
        printf("usage: cat [-benstv] [file ...]");
    }
    return 0;
}

void init(struct struc *fl) {
    fl->flag_b = 0;
    fl->flag_e = 0;
    fl->flag_n = 0;
    fl->flag_s = 0;
    fl->flag_t = 0;
    fl->flag_v = 0;
}

void parser(int argnum, char **arglist, struct struc *fl) {
    for (int count = 0; count < argnum; count++) {
        if (arglist[count][0] == '-' && arglist[count][1] != '-') {
            int len = strlen(arglist[1]);
            for (int i = 1; i < len; i++) {
                if (arglist[count][i] == 'b')
                    fl->flag_b = 1;
                if (arglist[count][i] == 'e') {
                    fl->flag_e = 1;
                    fl->flag_v = 1;
                }
                if (arglist[count][i] == 'n')
                    fl->flag_n = 1;
                if (arglist[count][i] == 's')
                    fl->flag_s = 1;
                if (arglist[count][i] == 't') {
                    fl->flag_t = 1;
                    fl->flag_v = 1;
                }
                if (arglist[count][i] == 'v')
                    fl->flag_v = 1;
                if (arglist[count][i] == 'E')
                    fl->flag_e = 1;
                if (arglist[count][i] == 'T')
                    fl->flag_t = 1;
            }
        } else if (arglist[count][0] == '-' && arglist[count][1] == '-') {
                if (!strcmp (arglist[count], "--number-nonblank"))
                    fl->flag_b = 1;
                if (!strcmp (arglist[count], "--number"))
                    fl->flag_n = 1;
                if (!strcmp (arglist[count], "--squeeze-blank"))
                    fl->flag_s = 1;
        }
    }
}

void print_ch(FILE *f, struct struc *fl) {
    if (fl->flag_b) {
        fl->flag_n = 0;
    }
    if (fl->flag_e || fl->flag_t) {
        fl->flag_v = 1;
    }
    char prev = '\0';
    char cur;
    int count = 0;
    int strnum = 0;

    while ((cur = getc(f)) != EOF) {
        if (fl->flag_s && cur == '\n' && (prev == '\n' || prev == '\0')) {
            if (count > 0) {
                continue;
            }
            count++;
        }
        if (fl->flag_s && cur != '\n') {
            count = 0;
        }
        if (fl->flag_n && (prev == '\0' || prev == '\n')) {
            strnum++;
            printf("%6d\t", strnum);
        }
        if (fl->flag_b && cur != '\n' && (prev == '\0' || prev == '\n')) {
            strnum++;
            printf("%6d\t", strnum);
        }
        if (fl->flag_v && cur >= 0 && cur <= 31 && cur != '\n' && cur != '\t') {
            char tmp = cur;
            printf("^");
            cur = tmp + 64;
        }
        if (fl->flag_e && cur == '\n') {
            printf("$");
        }
        if (fl->flag_t && cur == '\t') {
            printf("^I");
            continue;
        }
        prev = cur;
        printf("%c", cur);
    }
}
