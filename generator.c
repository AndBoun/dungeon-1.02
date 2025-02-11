//
// Created by Andrew Boun on 2/3/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <unistd.h>
#include "dungeon.h"


/**
 * Generates a corridor between two points in the dungeon grid.
 * The corridor can be horizontal, vertical, or diagonal based on random selection.
 * 
 * @param x1 The starting x-coordinate of the corridor
 * @param y1 The starting y-coordinate of the corridor
 * @param x2 The ending x-coordinate of the corridor
 * @param y2 The ending y-coordinate of the corridor
 * 
 * The function updates the global 'grid' array, converting ROCK cells to CORRIDOR
 * cells along the path. For diagonal movements, it ensures connectivity by adding
 * additional corridor cells when necessary.
 * 
 * Direction values:
 * 0 - horizontal movement
 * 1 - vertical movement
 * 2 - diagonal movement
 */
void generate_corridor(dungeon *d, int x1, int y1, int x2, int y2){
    int x = x1;
    int y = y1;

    // 0 for horizontal, 1 for vertical, 2 for diagonal
    int direction = rand() % 3;

    while (x != x2 || y != y2){
        if (direction == 0 && x != x2){
            if (x < x2) x++; 
            else x--;

        } else if (direction == 1 && y != y2){
            if (y < y2) y++;
            else y--;

        } else {
            if (x != x2){
                if (x < x2) x++;
                else x--;

                // Ensure diagonal movement is possible by only
                // up and down movements by adding an extra corridor
                if (d->grid[y][x].type == ROCK) {
                    d->grid[y][x].type = CORRIDOR;
                    d->grid[y][x].hardness = MIN_HARDNESS;
                }
                
            }

            if (y < y2){
                y++;
            } else if (y > y2){
                y--;
            }
        }

        if (d->grid[y][x].type == ROCK) {
            d->grid[y][x].type = CORRIDOR;
            d->grid[y][x].hardness = MIN_HARDNESS;
        }
    }
}

/**
 * Determines if a room can be placed at the specified location in the grid.
 * 
 * @param room The room structure containing x,y coordinates, width and height
 * @return 1 if room can be inserted, 0 otherwise
 * 
 * Checks two conditions:
 * 1. If the room dimensions would exceed the placeable grid boundaries
 * 2. If the room area (including 1-tile border) would overlap with existing floor tiles
 */
int can_insert_room(dungeon *d, Room room){

    // Check if room is within bounds
    // Do not check for '(x + width - 1) >= PLACEABLE' because we need to account for starting at 1
    // (room.width - 1) and (room.height - 1) to account for starting room at that index (x, y) inclusive
    if (room.x + (room.width - 1) > PLACABLE_WIDTH || room.y + (room.height - 1) > PLACABLE_HEIGHT){ 
        return 0;
    }

    // Check area of room and bordering area to see if it is empty
    for (int i = room.x - 1; i < room.x + room.width + 1; i++){
        for (int j = room.y - 1; j < room.y + room.height + 1; j++){
            if (d->grid[j][i].type == FLOOR){
                return 0; // Room overlaps with existing floor tiles
            }
        }
    }

    return 1; // Room can be inserted
}

/**
 * @brief Generates a room in the grid by filling it with floor tiles
 * 
 * Given a Room struct containing the position (x,y) and dimensions (width, height),
 * this function fills the corresponding grid cells with FLOOR characters.
 * The room is generated from its top-left corner (inclusive) to its bottom-right corner.
 * 
 * @param room A Room struct containing:
 *             - x: The x-coordinate of the top-left corner
 *             - y: The y-coordinate of the top-left corner
 *             - width: The width of the room
 *             - height: The height of the room
 * 
 * @pre The room must fit within the grid boundaries
 * @pre can_insert_room must return true for the given room
 * @pre grid must be properly initialized
 */
void generate_room(dungeon *d, Room room){
    for(int i = room.x; i < room.x + room.width; i++){
        for(int j = room.y; j < room.y + room.height; j++){
            d->grid[j][i].type = FLOOR;
            d->grid[j][i].hardness = MIN_HARDNESS;
        }
    }
}

/**
 * @brief Generates a random room with random dimensions and position
 * 
 * This function attempts to generate a random room within the dungeon grid.
 * It will make multiple attempts to place a room without overlapping existing rooms.
 * If too many attempts are made (MAX_ATTEMPTS), the function fails.
 * 
 * The room dimensions are randomly generated but must meet minimum size requirements
 * (MIN_ROOM_WIDTH and MIN_ROOM_HEIGHT). The room position is randomly placed within
 * the PLACABLE_WIDTH and PLACABLE_HEIGHT bounds.
 * 
 * @param idx Index where the generated room should be stored in the rooms array
 * @return true if room was successfully generated and placed
 * @return false if room could not be placed after maximum attempts
 */
bool generate_random_room(dungeon *d, int idx){
    Room room;
    int attempts = 0;

    do {
        attempts++;
        if (attempts >= MAX_ATTEMPTS) { // Reset grid if too many attempts
            return false;
        }

        room.x = (rand() % PLACABLE_WIDTH) + 1;
        room.y = (rand() % PLACABLE_HEIGHT) + 1;

        // Subract current coordinate from the maximum width and height
        // to get the maximum available width and height
        int max_width = PLACABLE_WIDTH - room.x;
        int max_height = PLACABLE_HEIGHT - room.y;

        int random_width = (max_width > 0) ? (rand() % max_width) : MIN_ROOM_WIDTH;
        int random_height = (max_height > 0) ? (rand() % max_height) : MIN_ROOM_HEIGHT;
        room.width = (random_width < MIN_ROOM_WIDTH) ? MIN_ROOM_WIDTH : random_width;
        room.height = (random_height < MIN_ROOM_HEIGHT) ? MIN_ROOM_HEIGHT : random_height;

        room.center_x = room.x + (room.width / 2);
        room.center_y = room.y + (room.height / 2);

    } while (!can_insert_room(d, room));

    d->rooms[idx] = room;
    generate_room(d, room);
    return true;
}

bool place_stair(dungeon *d, int x, int y, char stair) {
    if (x > 0 && x <= PLACABLE_WIDTH && y > 0 && y < PLACABLE_HEIGHT) {
        d->grid[y][x].type = stair;
        d->grid[y][x].hardness = MIN_HARDNESS;
        return true;
    }
    return false;
}

int generate_random_stair(dungeon *d, char stair){
    int x, y;
    do {
        x = (rand() % PLACABLE_WIDTH) + 1;
        y = (rand() % PLACABLE_HEIGHT) + 1;
    } while (d->grid[y][x].type == ROCK);

    d->grid[y][x].type =  stair;
    d->grid[y][x].hardness = MIN_HARDNESS;
    return 1;
}

void print_room_info(const dungeon *d){
    for (int i = 0; i < d->num_rooms; i++){
        printf("Room %d\n", i + 1);
        printf("x: %d\n", d->rooms[i].x);
        printf("y: %d\n", d->rooms[i].y);
        printf("width: %d\n", d->rooms[i].width);
        printf("height: %d\n", d->rooms[i].height);
        printf("center_x: %d\n", d->rooms[i].center_x);
        printf("center_y: %d\n", d->rooms[i].center_y);
        printf("\n");
    }
}

void print_hardness_info(const dungeon *d){
    for (int i = 0; i < DUNGEON_HEIGHT; i++){
        for (int j = 0; j < DUNGEON_WIDTH; j++){
            printf("%d ", d->grid[i][j].hardness);
        }
        printf("\n");
    }
}

// Prints grid with a border
void print_grid(const dungeon *d){
    for(int i = 0; i < DUNGEON_WIDTH + 2; i++){
        printf("%c", HORIZONTAL_BORDER);
    }
    printf("\n");

    for (int i = 0; i < DUNGEON_HEIGHT; i++){
        printf("%c", VERTICAL_BORDER);
        for (int j = 0; j < DUNGEON_WIDTH; j++){
            printf("%c", d->grid[i][j].type);
        }
        printf("%c", VERTICAL_BORDER);
        printf("\n");
    }

    for(int i = 0; i < DUNGEON_WIDTH + 2; i++){
        printf("%c", HORIZONTAL_BORDER);
    }
    printf("\n");
}

bool generate_random_dungeon(dungeon *d){
    int i, j;
    int num_rooms;

    // Discard first random value
    rand();

    bool success = false;

    // Generate Rooms
    do {
        // Initialize grid with ROCK
        for (i = 0; i < DUNGEON_HEIGHT; i++) {
            for (j = 0; j < DUNGEON_WIDTH; j++) {
                d->grid[i][j].type = ROCK;
                if (i == 0 || i == DUNGEON_HEIGHT - 1 || j == 0 || j == DUNGEON_WIDTH - 1) {
                    d->grid[i][j].hardness = MAX_HARDNESS;
                } else{
                    d->grid[i][j].hardness = rand() % (MAX_HARDNESS - 1 - MIN_HARDNESS + 1);
                }
            }
        }

        // Assume a successful room generation
        success = true;

        // Generate a random number of rooms to generate
        num_rooms = MIN_ROOMS + rand() % (MAX_ROOMS - MIN_ROOMS + 1);
        d->num_rooms = num_rooms;
        // printf("Number of rooms: %d\n", num_rooms);
        d->rooms = malloc(num_rooms * sizeof(Room));

        // Generate rooms
        for (i = 0; i < num_rooms; i++) {

            // If attempt limit is reached, reset grid and try again
            if (!generate_random_room(d, i)) {
                success = false;
                break;
            }
        }

        // If rooms were generated successfully, break out of loop
        if (success) break;

    } while (1);

    // Generate Corridors
    for (i = 0; i < num_rooms - 1; i++){
        generate_corridor(
            d,
            d->rooms[i].center_x, 
            d->rooms[i].center_y, 
            d->rooms[i + 1].center_x, 
            d->rooms[i + 1].center_y
        );
    }

    // Generate Stairs
    int num_up_stairs = MIN_UP_STAIRS + rand() % (MAX_UP_STAIRS - MIN_UP_STAIRS + 1);
    for (i = 0; i < num_up_stairs; i++){
        generate_random_stair(d, UP_STAIRS);
    }

    int num_down_stairs = MIN_DOWN_STAIRS + rand() % (MAX_DOWN_STAIRS - MIN_DOWN_STAIRS + 1);
    for (i = 0; i < num_down_stairs; i++){
        generate_random_stair(d, DOWN_STAIRS);
    }


    // print_room_info(num_rooms);
    
    print_grid(d);
    return true;
}