//
// Created by Andrew Boun on 2/11/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include "dungeon.h"
#include "load_save.h"

int main() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand(tv.tv_usec ^ getpid());

    Dungeon d;
    init_dungeon(&d);

    // if (generate_random_dungeon(&d)) {
    //     print_grid(&d);
    // } else {
    //     printf("Dungeon generation failed.\n");
    // }


    load_save(&d);
    print_grid(&d);

    // save(&d);

    free(d.rooms);
    free(d.up_stairs);
    free(d.down_stairs);

    return 0;
}