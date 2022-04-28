CC=gcc
FLAGS= #-Wall -Wextra -Werror -std=c11
LINUX=-lrt -lpthread -lm -D_GNU_SOURCE
OS=$(shell uname -s)
DIRCAT=./

all: clean grep

grep:
ifeq ($(OS), Darwin)
	$(CC) $(FLAGS) $(DIRCAT)grep.c -o grep
else
	$(CC) $(LINUX) $(DIRCAT)grep.c -o grep
endif

test: grep
	./grep ../test1.txt ../test.txt -e b >> my_grep_test.txt
	grep ../test1.txt ../test.txt -e b >> orig_grep_test.txt
	./grep b ../test1.txt ../test.txt >> my_grep_test.txt
	grep b ../test1.txt ../test.txt >> orig_grep_test.txt
	./grep -i b ../test1.txt >> my_grep_test.txt
	grep -i b ../test1.txt >> orig_grep_test.txt
	./grep -i b ../test1.txt ../test.txt >> my_grep_test.txt
	grep -i b ../test1.txt ../test.txt >> orig_grep_test.txt
	./grep -v b ../test1.txt ../test.txt >> my_grep_test.txt
	grep -v b ../test1.txt ../test.txt >> orig_grep_test.txt
	./grep -c b ../test1.txt >> my_grep_test.txt
	grep -c b ../test1.txt >> orig_grep_test.txt
	./grep -l b ../test1.txt ../test.txt >> my_grep_test.txt
	grep -l b ../test1.txt ../test.txt >> orig_grep_test.txt
	./grep -n 1 ../test1.txt >> my_grep_test.txt
	grep -n 1 ../test1.txt >> orig_grep_test.txt
	./grep -h b ../test1.txt ../test.txt >> my_grep_test.txt
	grep -h b ../test1.txt ../test.txt >> orig_grep_test.txt
	./grep -f ../pat.txt ../test1.txt >> my_grep_test.txt
	grep -f ../pat.txt ../test1.txt >> orig_grep_test.txt
	gcc tests_grep.c -o test_grep -lcheck
	./test_grep

clean:
	rm -rf *.out grep test_grep my_grep_test.txt orig_grep_test.txt

rebuild: clean all
