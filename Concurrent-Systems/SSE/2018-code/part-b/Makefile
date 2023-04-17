all: code-segments.h code-segment-2.c code-segment-3.c code-segment-4.c main.c
	gcc -g -O0 -march=native code-segments.h code-segment-2.c \
	code-segment-3.c code-segment-4.c main.c -o main 

run: all 
	./main

clean:
	rm main 
