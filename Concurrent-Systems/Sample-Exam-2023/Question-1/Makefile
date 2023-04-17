all: csu33014-annual-q1-code.c csu33014-annual-q1-code.h csu33014-annual-q1-main.c
	gcc -msse4 -O2 -o vec csu33014-annual-q1-code.c csu33014-annual-q1-main.c

debug: csu33014-annual-q1-code.c csu33014-annual-q1-code.h csu33014-annual-q1-main.c
	gcc -msse4 -g -o vec csu33014-annual-q1-code.c csu33014-annual-q1-main.c

test: all
	./vec
