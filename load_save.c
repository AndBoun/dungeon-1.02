//
// Created by Andrew Boun on 2/10/25.
//

#include <stdio.h>
#include <stdlib.h>
#include "load_save.h"

int init_load_save(LoadSave *ls){

}

int main(){
    char *home;
    char *dungeon_file;
    int dungeon_file_length;

    home = getenv("HOME");
    dungeon_file_length = strlen(home) + strlen("/.rlg327/dungeon") + 1; // +1 for the null byte
    dungeon_file = malloc(dungeon_file_length * sizeof (*dungeon_file));
    strcpy(dungeon_file, home);
    strcat(dungeon_file, "/.rlg327/dungeon");
}