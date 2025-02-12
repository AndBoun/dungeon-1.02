//
// Created by Andrew Boun on 2/3/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dungeon.h"


/**
 * Generates a corridor between two points in the Dungeon grid.
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
void generate_corridor(Dungeon *d, int x1, int y1, int x2, int y2){
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

int place_player(Dungeon *d){
    int x, y;
    do {
        x = rand() % PLACABLE_WIDTH + 1;
        y = rand() % PLACABLE_HEIGHT + 1;
    } while (d->grid[y][x].type == CORRIDOR || d->grid[y][x].type == ROCK);

    d->pc_x = x;
    d->pc_y = y;
    d->grid[y][x].type = PLAYER;

    return 1;
}

bool generate_random_dungeon(Dungeon *d){
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
                    d->grid[i][j].hardness = rand() % (MAX_HARDNESS - 1 - MIN_HARDNESS) + 1;
                }
            }
        }

        // Assume a successful room generation
        success = true;

        // Generate a random number of rooms to generate
        num_rooms = MIN_ROOMS + rand() % (MAX_ROOMS - MIN_ROOMS + 1);
        d->num_rooms = num_rooms;
        d->current_room_idx = 0;
        // printf("Number of rooms: %d\n", num_rooms);
        d->rooms = malloc(num_rooms * sizeof(Room));

        // Generate rooms
        for (i = 0; i < num_rooms; i++) {

            // If attempt limit is reached, reset grid and try again
            if (!generate_random_room(d)) {
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
    d->num_up_stairs = MIN_UP_STAIRS + rand() % (MAX_UP_STAIRS - MIN_UP_STAIRS + 1);
    d->up_stairs = malloc(d->num_up_stairs * sizeof(Stair));
    for (i = 0; i < d->num_up_stairs; i++){
        generate_random_stair(d, UP_STAIRS, i);
    }

    d->num_down_stairs = MIN_DOWN_STAIRS + rand() % (MAX_DOWN_STAIRS - MIN_DOWN_STAIRS + 1);
    d->down_stairs = malloc(d->num_down_stairs * sizeof(Stair));
    for (i = 0; i < d->num_down_stairs; i++){
        generate_random_stair(d, DOWN_STAIRS, i);
    }

    place_player(d);

    return true;
}