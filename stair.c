//
// Created by Andrew Boun on 2/11/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dungeon.h"

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