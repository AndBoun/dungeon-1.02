//
// Created by Andrew Boun on 2/11/25.
//

#ifndef LOAD_SAVE_H
#define LOAD_SAVE_H

typedef struct {
    char *home;
    char *dungeon_file;
    int dungeon_file_length;
} LoadSave;

int init_load_save(LoadSave *ls);

#endif