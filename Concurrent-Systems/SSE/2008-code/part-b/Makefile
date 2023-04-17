all: code-segments.h code-segment-2.c main.c 
	gcc -g -O0 -march=native code-segments.h \
	code-segment-2.c main.c -o main -lm

run: all 
	./main

clean:
	rm main 
