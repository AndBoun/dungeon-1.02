//
// Created by Andrew Boun on 2/11/25.
//

#ifndef DUNGEON_H
#define DUNGEON_H

#include <stdbool.h>

// Character definitions for the dungeon grid
#define ROCK ' '
#define FLOOR '.'
#define CORRIDOR '#'
#define DOWN_STAIRS '>'
#define UP_STAIRS '<'

// Border characters for the dungeon grid
#define HORIZONTAL_BORDER '-'
#define VERTICAL_BORDER '|'

#define DUNGEON_WIDTH 80
#define DUNGEON_HEIGHT 21

#define PLACABLE_HEIGHT (DUNGEON_HEIGHT - 2)
#define PLACABLE_WIDTH (DUNGEON_WIDTH - 2)

#define MIN_ROOMS 6
#define MAX_ROOMS 120

#define MIN_ROOM_WIDTH 4
#define MIN_ROOM_HEIGHT 3

#define MIN_DOWN_STAIRS 1
#define MAX_DOWN_STAIRS 4
#define MIN_UP_STAIRS 1
#define MAX_UP_STAIRS 4

#define MIN_HARDNESS 0
#define MAX_HARDNESS 255

// Maximum number of attempts to generate a room
// If the number of attempts exceeds this value,
// The grid will reset and try again
#define MAX_ATTEMPTS 2000

typedef struct {
    int x, y;       // top-left coordinate (not including border)
    int width, height;
    int center_x, center_y;
} Room;

typedef struct {
    int hardness;
    char type;
} floor_tile;

typedef struct dungeon {
    floor_tile grid[DUNGEON_HEIGHT][DUNGEON_WIDTH];
    Room* rooms;       // Dynamically allocated array of rooms
    int num_rooms;     // Number of rooms in the dungeon
} dungeon;


// Function prototypes updated to work with dungeon struct
void generate_corridor(dungeon *d, int x1, int y1, int x2, int y2);
int can_insert_room(dungeon *d, Room room);
void generate_room(dungeon *d, Room room);
bool generate_random_room(dungeon *d, int idx);
int generate_random_stair(dungeon *d, char stair);
bool place_stair(dungeon *d, int x, int y, char stair);

void print_room_info(const dungeon *d);
void print_hardness_info(const dungeon *d);
void print_grid(const dungeon *d);

bool generate_random_dungeon(dungeon *d);

#endif