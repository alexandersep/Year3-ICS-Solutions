all: csu33014-annual-q2-code.c csu33014-annual-q2-main.c csu33014-annual-q2-code.h linkedlist.c linkedlist.h hashtable.c hashtable.h
	gcc -O2 -fopenmp -o finddups csu33014-annual-q2-code.c csu33014-annual-q2-main.c linkedlist.c linkedlist.h hashtable.c hashtable.h

debug: csu33014-annual-q2-code.c csu33014-annual-q2-main.c csu33014-annual-q2-code.h linkedlist.c linkedlist.h hashtable.c hashtable.h
	gcc -g -fopenmp -o finddups csu33014-annual-q2-code.c csu33014-annual-q2-main.c linkedlist.c linkedlist.h hashtable.c hashtable.h

sanitize: csu33014-annual-q2-code.c csu33014-annual-q2-main.c csu33014-annual-q2-code.h linkedlist.c linkedlist.h hashtable.c hashtable.h
	gcc -g -fsanitize=address -fopenmp -o finddups csu33014-annual-q2-code.c csu33014-annual-q2-main.c linkedlist.c linkedlist.h hashtable.c hashtable.h

test: all
	./finddups 2000

time: all
	./finddups 20000

clean: finddups
	rm -f finddups
