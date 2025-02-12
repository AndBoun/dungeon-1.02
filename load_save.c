//
// Created by Andrew Boun on 2/10/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "dungeon.h"
#include "load_save.h"

int init_load_save(LoadSave *ls, const char *mode) {
    ls->home = getenv("HOME");
    ls->dungeon_file_length = strlen(ls->home) + strlen("/.rlg327/dungeon") + 1;
    ls->dungeon_file = malloc(ls->dungeon_file_length * sizeof(*(ls->dungeon_file)));
    strcpy(ls->dungeon_file, ls->home);
    strcat(ls->dungeon_file, "/.rlg327/dungeon");

    ls->f = fopen(ls->dungeon_file, mode);
    if (!ls->f) {
        fprintf(stderr, "Error: Could not open file %s\n", ls->dungeon_file);
        return 1;
    }
    
    return 0;
}

char* load_marker(LoadSave *ls) {
    char* marker = malloc(13);  // 12 bytes + 1 for null terminator

    // Read exactly 12 bytes
    if (fread(marker, sizeof(char), 12, ls->f) != 12) {
        fprintf(stderr, "Error: Could not read marker from file\n");
        fclose(ls->f);
        free(marker);
        return NULL;
    }

    // Null terminate the string
    marker[12] = '\0';

    return marker;
}

int load_version(LoadSave *ls) {
    uint32_t version;

    // Read exactly 4 bytes
    if (fread(&version, sizeof(version), 1, ls->f) != 1) {
        fprintf(stderr, "Error: Could not read version from file\n");
        fclose(ls->f);
        return 1;
    }

    version = be32toh(version);

    return version;
}

int load_size(LoadSave *ls) {
    uint32_t size;

    // Read exactly 4 bytes
    if (fread(&size, sizeof(size), 1, ls->f) != 1) {
        fprintf(stderr, "Error: Could not read size from file\n");
        fclose(ls->f);
        return 1;
    }

    size = be32toh(size);

    return size;
}

int load_pc(LoadSave *ls, Dungeon *d) {
    u_int8_t pc[2]; // player character

    // Read exactly 2 bytes
    if (fread(pc, sizeof(u_int8_t), 2, ls->f) != 2) {
        fprintf(stderr, "Error: Could not read pc from file\n");
        fclose(ls->f);
        return 1;
    }
    d->pc_x = (int) pc[0];
    d->pc_y = (int) pc[1];

    d->grid[d->pc_y][d->pc_x].type = PLAYER;

    return 0;
}

int load_hardness(LoadSave *ls, Dungeon *d) {
    uint8_t *hardness = malloc(1680); // 1680 bytes for hardness

    // Read exactly 1680 bytes
    if (fread(hardness, sizeof(u_int8_t), 1680, ls->f) != 1680) {
        fprintf(stderr, "Error: Could not read hardness from file\n");
        fclose(ls->f);
        return 1;
    }
    for (int i = 0; i < 1680; i++){
        FloorTile *ft = &d->grid[i / DUNGEON_WIDTH][i % DUNGEON_WIDTH];
        ft->hardness = hardness[i];
        ft->type = ROCK;
    }

    return 0;
}

int load_num_rooms(LoadSave *ls, Dungeon *d) {
    uint16_t num_rooms;
    
    if (fread(&num_rooms, sizeof(uint16_t), 1, ls->f) != 1) {
        fprintf(stderr, "Error: Could not read num_rooms from file\n");
        fclose(ls->f);
        return 1;
    }

    num_rooms = be16toh(num_rooms);
    d->num_rooms = (int) num_rooms;

    return 0;
}

int load_rooms(LoadSave *ls, Dungeon *d, int r){
    u_int8_t rooms[r * 4]; // 4 bytes per room

    if(fread(rooms, sizeof(u_int8_t), r * 4, ls->f) != r * 4){
        fprintf(stderr, "Error: Could not read rooms from file\n");
        fclose(ls->f);
        return 1;
    }

    d->rooms = malloc(r * sizeof(Room));
    for (int i = 0; i < r; i++){
        d->rooms[i].x = (int) rooms[i * 4];
        d->rooms[i].y = (int) rooms[i * 4 + 1];
        d->rooms[i].width = (int) rooms[i * 4 + 2];
        d->rooms[i].height = (int) rooms[i * 4 + 3];
        d->rooms[i].center_x = d->rooms[i].x + d->rooms[i].width / 2;
        d->rooms[i].center_y = d->rooms[i].y + d->rooms[i].height / 2;

        generate_room(d, d->rooms[i]);
    }

    d->grid[d->pc_y][d->pc_x].type = PLAYER;

    return 0;
}

int load_num_up_stairs(LoadSave *ls, Dungeon *d) {
    uint16_t num_up_stairs;

    if (fread(&num_up_stairs, sizeof(uint16_t), 1, ls->f) != 1) {
        fprintf(stderr, "Error: Could not read num_up_stairs from file\n");
        fclose(ls->f);
        return 1;
    }

    num_up_stairs = be16toh(num_up_stairs);
    d->num_up_stairs = (int) num_up_stairs;

    return 0;
}

int load_up_stairs(LoadSave *ls, Dungeon *d, int u) {
    u_int8_t up_stairs[u * 2]; // 2 bytes per up_stairs

    if (fread(up_stairs, sizeof(u_int8_t), u * 2, ls->f) != u * 2) {
        fprintf(stderr, "Error: Could not read up_stairs from file\n");
        fclose(ls->f);
        return 1;
    }

    for (int i = 0; i < u; i++) {
        d->grid[up_stairs[i * 2 + 1]][up_stairs[i * 2]].type = UP_STAIRS;
    }

    return 0;
}

int load_num_down_stairs(LoadSave *ls, Dungeon *d) {
    uint16_t num_down_stairs;

    if (fread(&num_down_stairs, sizeof(uint16_t), 1, ls->f) != 1) {
        fprintf(stderr, "Error: Could not read num_down_stairs from file\n");
        fclose(ls->f);
        return 1;
    }

    num_down_stairs = be16toh(num_down_stairs);
    d->num_down_stairs = (int) num_down_stairs;

    return 0;
}

int load_down_stairs(LoadSave *ls, Dungeon *d, int down) {
    u_int8_t down_stairs[down * 2]; // 2 bytes per down_stairs

    if (fread(down_stairs, sizeof(u_int8_t), down * 2, ls->f) != down * 2) {
        fprintf(stderr, "Error: Could not read down_stairs from file\n");
        fclose(ls->f);
        return 1;
    }

    for (int i = 0; i < down; i++) {
        d->grid[down_stairs[i * 2 + 1]][down_stairs[i * 2]].type = DOWN_STAIRS;
    }

    return 0;
}

int fill_in_corridors(Dungeon *d) {
    for (int i = 0; i < DUNGEON_HEIGHT; i++) {
        for (int j = 0; j < DUNGEON_WIDTH; j++) {
            if (d->grid[i][j].type == ROCK && d->grid[i][j].hardness == 0) {
                d->grid[i][j].type = CORRIDOR;
            }
        }
    }

    return 0;
}