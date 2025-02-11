all: main

main: main.o generator.o
	gcc -g main.o generator.o -o main

generator.o: generator.c dungeon.h
	gcc -g generator.c -c -Wall -Werror

main.o: main.c dungeon.h
	gcc -g main.c -c -Wall -Werror

clean:
	rm -f *.o *~ main