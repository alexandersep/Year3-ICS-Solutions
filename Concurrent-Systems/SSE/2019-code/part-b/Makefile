all: code-segments.h code-segment-2.c main.c
	gcc -g -O0 -march=native code-segments.h \
	code-segment-2.c main.c -o main 

run: all 
	./main

clean: main
	rm -f main 
