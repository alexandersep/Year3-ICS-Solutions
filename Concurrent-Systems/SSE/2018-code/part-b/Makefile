# Westmere has been chosen, as stoker runs that architecture
# If you want to run it on your computer use -march=native
code-segment-2: 
	gcc -g -O0 -march=westmere code-segment-2.c -o code-segment-2

code-segment-3: 
	gcc -g -O0 -march=westmere code-segment-3.c -o code-segment-3 

code-segment-4: 
	gcc -g -O0 -march=westmere code-segment-4.c -o code-segment-4

run-code-segment-2: code-segment-2
	./code-segment-2

run-code-segment-3: code-segment-3
	./code-segment-3

run-code-segment-4: code-segment-4
	./code-segment-4

all: code-segment-2 code-segment-3 code-segment-4

run: all
	./code-segment-2
	./code-segment-3
	./code-segment-4

clean:
	rm code-segment-2 code-segment-3 code-segment-4 
