//
// Created by Andrew Boun on 2/11/25.
//

#include "dungeon.h"
#include <stdlib.h>

// Function to initialize a dungeon instance
void init_dungeon(Dungeon *d) {
    d->rooms = NULL;
    d->num_rooms = 0;
    d->current_room_idx = 0;
    d->pc_x = 0;
    d->pc_y = 0;
    d->num_up_stairs = 0;
    d->num_down_stairs = 0;
    d->up_stairs = NULL;
    d->down_stairs = NULL;
    d->current_up_stair_idx = 0;
    d->current_down_stair_idx = 0;
}