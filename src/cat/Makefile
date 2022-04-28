CC=gcc
FLAGS= -Wall -Wextra -Werror -std=c11
LINUX=-lrt -lpthread -lm -D_GNU_SOURCE
OS=$(shell uname -s)
DIRCAT=./

all: clean cat

cat:
ifeq ($(OS), Darwin)
	$(CC) $(FLAGS) $(DIRCAT)cat.c -o cat
else
	$(CC) $(LINUX) $(DIRCAT)cat.c -o cat
endif

test: cat
	./cat -benst ../test1.txt ../test.txt >> my_test.txt
	cat -benst ../test1.txt ../test.txt >> orig_test.txt
	gcc tests_cat.c -o test_cat -lcheck
	./test_cat

clean:
	rm -rf *.out cat test_cat orig_test.txt my_test.txt

rebuild: clean all