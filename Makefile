CFLAGS=-c -std=gnu99 -Wall -pedantic

main: main.o miller-rabin.o
	gcc -g main.o miller-rabin.o -o main

main.o: main.c miller-rabin.c miller-rabin.h
	gcc $(CFLAGS) main.c

miller-rabin.o: miller-rabin.c miller-rabin.h
	gcc $(CFLAGS) miller-rabin.c

clean:
	rm -f main *.exe *.o