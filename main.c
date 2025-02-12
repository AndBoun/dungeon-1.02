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
    // free(d.rooms);

    int r, u, down;

    LoadSave ls;
    init_load_save(&ls, "rb");
    printf("Home: %s\n", ls.home);
    printf("Dungeon file: %s\n", ls.dungeon_file);

    char* marker = load_marker(&ls);
    printf("Marker: %s\n", marker);
    if (strcmp(marker, EXPECTED_MARKER) != 0) {
        fprintf(stderr, "Error: Invalid marker\n");
        free(marker);
        fclose(ls.f);
        free(ls.dungeon_file);
        return 1;
    }
    free(marker);

    uint32_t version = load_version(&ls);
    printf("Version: %u\n", version);

    uint32_t size = load_size(&ls);
    printf("Size: %u\n", size);

    load_pc(&ls, &d); // player character
    printf("PC: x: %u \t y: %u \n", d.pc_x, d.pc_y);

    load_hardness(&ls, &d);
    // print_hardness_info(&d);

    load_num_rooms(&ls, &d);
    r = d.num_rooms;
    printf("Number of rooms: %u\n", d.num_rooms);

    load_rooms(&ls, &d, r);
    print_grid(&d);

    load_num_up_stairs(&ls, &d);
    u = d.num_up_stairs;
    printf("Number of up stairs: %u\n", d.num_up_stairs);

    load_up_stairs(&ls, &d, u);
    print_grid(&d);

    load_num_down_stairs(&ls, &d);
    down = d.num_down_stairs;
    printf("Number of down stairs: %u\n", d.num_down_stairs);

    load_down_stairs(&ls, &d, down);
    print_grid(&d);

    fill_in_corridors(&d);
    print_grid(&d);





    fclose(ls.f);
    free(ls.dungeon_file);

    return 0;
}