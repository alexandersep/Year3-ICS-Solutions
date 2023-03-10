# Westmere has been chosen, as stoker runs that architecture
# If you want to run it on your computer use -march=native
code-segment-1: code-segment-1.c
	gcc -g -O0 -march=westmere code-segment-1.c -o code-segment-1

code-segment-2: code-segment-2.c
	gcc -g -O0 -march=westmere code-segment-2.c -o code-segment-2

run-code-segment-1: code-segment-1
	./code-segment-1

run-code-segment-2: code-segment-2
	./code-segment-2

all: code-segment-1 code-segment-2

run: all
	./code-segment-1
	./code-segment-2
clean:
	rm code-segment-1 code-segment-2
