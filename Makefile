all: dungeon_generator

dungeon_generator: dungeon_generator.c
	gcc dungeon_generator.c -o dungeon_generator -Wall -Werror

clean:
	rm -f dungeon_generator *~