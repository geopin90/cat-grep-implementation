#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include "grep.h"

int main(int argc, char **argv) {
    if (argc > 1) {
        struct flags fl;
        init(&fl);
        pat_init(&fl, argc);
        file_parse(argc, argv, &fl);
        parser(argc, argv, &fl);
        compare(argc, argv, &fl);
        pat_destr(&fl, argc);
    } else {
        fprintf(stderr, "usage: grep [options] template [file_name]\n");
    }
    return 0;
}

void init(struct flags *fl) {
    fl->flag_e = 0;
    fl->flag_i = 0;
    fl->flag_v = 0;
    fl->flag_c = 0;
    fl->flag_l = 0;
    fl->flag_n = 0;
    fl->flag_h = 0;
    fl->flag_s = 0;
    fl->flag_f = 0;
    fl->temp_count = 0;
    fl->file_count = 0;
    fl->struct_size = 30;
    fl->print_flag_l = 0;
}

void file_parse(int argnum, char **arglist, struct flags *fl) {
    FILE *check;
    for (int i = 1; i < argnum; i++) {
        if (arglist[i][0] != '-') {
            if ((check = fopen(arglist[i], "rt")) != NULL) {
                if (arglist[i-1][0] != '-' && arglist[i-1][1] != 'f') {
                    fl->file_count++;
                }
                fclose(check);
            }
        }
    }
}

void pat_init(struct flags *fl, int argnum) {
    fl->pat_e = (char**)malloc(argnum * sizeof(char*));
    for (int i = 0; i < argnum; i++) {
        fl->pat_e[i] = (char*)malloc(fl->struct_size * sizeof(char));
    }
}

void pat_realloc(struct flags *fl, int lsize) {
    fl->struct_size += lsize;
    fl->pat_e[fl->temp_count] = (char*)realloc(fl->pat_e[fl->temp_count], fl->struct_size);
}

void pat_destr(struct flags *fl, int argnum) {
    for (int i = 0; i < argnum; i++) {
        free(fl->pat_e[i]);
    }
    free(fl->pat_e);
}

char *e_strcpy(char *dest, const char *src) {
    size_t len = strlen(src) - 2;
    for (size_t i = 0, j = 2; i < len + 1; i++, j++) {
        dest[i] = src[j];
    }
    return dest;
}

void parser(int argnum, char **arglist, struct flags *fl) {
    int count = 1, check = 0;
    while (count < argnum) {
        if (arglist[count][0] == '-') {
            if (arglist[count][1] == 'e' && arglist[count][2] == '\0') {
                int len = strlen(arglist[count + 1]);
                if (len > fl->struct_size) pat_realloc(fl, len);
                strcpy(fl->pat_e[fl->temp_count++], arglist[count + 1]);
                count++;
            } else if (arglist[count][1] == 'e' && arglist[count][2] != '\0') {
                e_strcpy(fl->pat_e[fl->temp_count++], arglist[count]);
            } else if (arglist[count][1] == 'i') { fl->flag_i = 1;
            } else if (arglist[count][1] == 'v') { fl->flag_v = 1;
            } else if (arglist[count][1] == 'c') { fl->flag_c = 1;
            } else if (arglist[count][1] == 'l') { fl->flag_l = 1;
            } else if (arglist[count][1] == 'n') { fl->flag_n = 1;
            } else if (arglist[count][1] == 'h') { fl->flag_h = 1;
            } else if (arglist[count][1] == 's') { fl->flag_s = 1;
            } else if (arglist[count][1] == 'f') { fl->flag_f = 1;}
        } else if (!fl->flag_e) {
            FILE *pat_test;
            if (arglist[count-1][0] == '-' && arglist[count-1][1] == 'f') {
                if ((pat_test = fopen(arglist[count], "rt")) != NULL) {
                    flag_f(pat_test, fl);
                    fclose(pat_test);
                } else {
                    fprintf(stderr, "grep: %s: No such file or directory\n", arglist[count]);
                }
            } else if ((pat_test = fopen(arglist[count], "rt")) == NULL) {
                if (check == 0) {
                    int len = strlen(arglist[count]);
                    if (len > fl->struct_size) {pat_realloc(fl, len);}
                    strcpy(fl->pat_e[fl->temp_count++], arglist[count]);
                    if (count == 1)
                        check++;
                }
            } else {fclose(pat_test);}
        }
        count++;
    }
}

void flag_f(FILE *f, struct flags *fl) {
    char *str = NULL;
    size_t len = 0;
    while (getline(&str, &len, f) != EOF) {
        strcpy(fl->pat_e[fl->temp_count], str);
        int patlen = strlen(fl->pat_e[fl->temp_count]);
        for (int i = 0; i < patlen; i++) {
            if (fl->pat_e[fl->temp_count][i] == '\n')
                fl->pat_e[fl->temp_count][i] = '\0';
        }
        fl->temp_count++;
    }
    free(str);
}

void compare(int argnum, char **arglist, struct flags *fl) {
    FILE *f;
    for (int j = 1; j < argnum ; j++) {
        if (arglist[j][0] != '-') {
            fl->print_flag_l = 0;
            if ((f = fopen(arglist[j], "rt")) != NULL) {
                if (arglist[j-1][0] != '-' && arglist[j-1][1] != 'f') {
                    char *buf = NULL;
                    size_t len = 0;
                    int str_count = 0, f_str_count = 0;
                    while (getline(&buf, &len, f) != EOF) {
                        f_str_count++;
                        display(buf, fl, arglist[j], &str_count, f_str_count);
                        if (fl->print_flag_l)
                            break;
                    }
                    if (fl->flag_c && fl->file_count > 1)
                        printf("%s:%d\n", arglist[j], str_count);
                    else if (fl->flag_c && fl->file_count <= 1)
                        printf("%d\n", str_count);
                    free(buf);
                }
                fclose(f);
            } else if (!fl->flag_s && fl->temp_count && j > 1 &&
            arglist[j-1][0] != '-' && arglist[j-1][1] != 'e' && arglist[j-1][2] != '\0') {
                fprintf(stderr, "grep: %s: No such file or directory\n", arglist[j]);
            }
        }
    }
}

void display(char *buf, struct flags *fl, char *argument, int *str_count, int f_str_count) {
    int iflag = 0;
    if (fl->flag_i) {
        iflag = REG_ICASE;
    }
    for (int i = 0; i < fl->temp_count; i++) {
        regex_t regex;
        regcomp(&regex, fl->pat_e[i], iflag);
        int result = regexec(&regex, buf, 0, NULL, 0);
        if (!fl->flag_l && fl->flag_c && !result) {
            *str_count += 1;
        } else if (!fl->flag_c) {
            if (!result && fl->flag_l) {
                printf("%s\n", argument);
                fl->print_flag_l++;
            } else if ((result && fl->flag_v) || (!result && !fl->flag_v)) {
                if (!fl->flag_h && !fl->flag_n && fl->file_count > 1) {
                    printf("%s:%s", argument, buf);
                } else if (!fl->flag_h && fl->flag_n && fl->file_count > 1) {
                    printf("%s:%d:%s", argument, f_str_count, buf);
                } else if (!fl->flag_n && (fl->flag_h || fl->file_count == 1)) {
                    printf("%s", buf);
                } else if (fl->flag_n && (fl->flag_h || fl->file_count == 1)) {
                    printf("%d:%s", f_str_count, buf);
                }
            }
        }
        regfree(&regex);
    }
}
