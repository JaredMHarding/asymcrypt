CFLAGS=-c -std=gnu99 -Wall -pedantic

asymcrypt: main.o asymcrypt.o miller-rabin.o
	gcc -g main.o asymcrypt.o miller-rabin.o -o asymcrypt

main.o: main.c asymcrypt.h miller-rabin.h
	gcc $(CFLAGS) main.c

asymcrypt.o: asymcrypt.c asymcrypt.h miller-rabin.h
	gcc $(CFLAGS) asymcrypt.c

miller-rabin.o: miller-rabin.c miller-rabin.h
	gcc $(CFLAGS) miller-rabin.c

clean:
	rm -f asymcrypt *.exe *.o