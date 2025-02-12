all: main

main: main.o generator.o print.o room.o stair.o dungeon.o load_save.o
	gcc -g main.o generator.o print.o room.o stair.o dungeon.o load_save.o -o main

load_save.o: load_save.c load_save.h dungeon.h
	gcc -g load_save.c -c -Wall -Werror

print.o: print.c dungeon.h
	gcc -g print.c -c -Wall -Werror

room.o: room.c dungeon.h
	gcc -g room.c -c -Wall -Werror

stair.o: stair.c dungeon.h
	gcc -g stair.c -c -Wall -Werror

generator.o: generator.c dungeon.h
	gcc -g generator.c -c -Wall -Werror

dungeon.o: dungeon.c dungeon.h
	gcc -g dungeon.c -c -Wall -Werror

main.o: main.c dungeon.h load_save.h
	gcc -g main.c -c -Wall -Werror



clean:
	rm -f *.o *~ main