.PHONY: clean

sourcediff: main.c parser.c parser.h hash.c hash.h select.c select.h compare.c compare.h quicksort.c quicksort.c mktmp.c mktmp.h
	gcc -o sourcediff main.c parser.c hash.c select.c compare.c quicksort.c mktmp.c

clean: 
	rm -f sourcediff
