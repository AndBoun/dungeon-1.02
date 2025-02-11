//
// Created by Andrew Boun on 2/11/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include "dungeon.h"

int main() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand(tv.tv_usec ^ getpid());

    dungeon d;
    init_dungeon(&d);

    if (generate_random_dungeon(&d)) {
        print_grid(&d);
    } else {
        printf("Dungeon generation failed.\n");
    }

    free(d.rooms);

    return 0;
}