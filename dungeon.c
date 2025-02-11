//
// Created by Andrew Boun on 2/11/25.
//

#include "dungeon.h"
#include <stdlib.h>

// Function to initialize a dungeon instance
void init_dungeon(dungeon *d) {
    d->rooms = NULL;
    d->num_rooms = 0;
    d->current_room_idx = 0;
    // Initialize other members as needed
}