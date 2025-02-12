# How To Run The Project:

 Create a `.rlg327` folder in your home directory:
```bash
mkdir ~/.rlg327
```

Build the project:
```bash
make
```

Run the project with flags:
```
./main --save --load
```


> # Dungeon File
> Whenever a save or load occurs, it uses the file called `dungeon`. This doesn't have any extension and cannot have one for loading to work.


# About The Project

## Constraints:
- All file inputs are assumed to be valid
- It only loads from one file: `dungeon` in the `~/.rlg327` directory
- It only writes to one file: `dungeon` `~/.rlg327` directory
- Using both `--save` `--load` flags at the same time, will always load, then save the dungeon, regardless of which ever order you put the flags.

## How The Project Runs:
The `load_save.h` header holds the structure for both loading and saving dungeons. First, in the main method, we check the flags the program in ran with, then decide to save, load, or do both.
If we load, we use the `load_save` method in the `load.c` file to go through each segment of the binary file, converting to Big Endian when ever necessary. Through this we slowly build up the dungeon, and eventually print it with statistics. 

When saving, we first generate a randomized dungeon, then call the `save` method in `save.c` to begin writing the contents to binary. We write the necessary headers, calculate the size in bytes, and build write in the same way we originally loaded the data from.


# Example Output:
```plain
CS327@CS327_dungeon1.02% ./main --load --save
Home: /Users/CS3270
Dungeon file: /Users/CS3270/.rlg327/dungeon
Marker: RLG327-S2025
Version: 0
Size: 1750
PC: x: 21        y: 16 
Number of rooms: 9
Number of up stairs: 2
Number of down stairs: 1
----------------------------------------------------------------------------------
|                                                                                |
|                                           ....                                 |
|         ##################################....                                 |
|         #                            .... .... ...................      ....   |
|         #                            ....      ...................      ....   |
|         ######.........##############....######...................      ....   |
|       ....    .........              ....      ...................      ....   |
|       ....    .........                        ...................      ....   |
|       ....    .........                        ...................######....   |
|       ....    .........                        ...................   ###....   |
|       ....    .........                        ...................  ##  ....   |
|       ....    .........##<###############      ................>.. ##   ....   |
|       ....    .........                 #      ...................##    ....   |
|       ....    .........                 #                        ##     ....   |
|       ....    .........                 #                       ##      ....   |
|               .........                 #                 .........            |
|               ......@..     ........................######.........            |
|                   #         ........................      .........   <...     |
|                   ##########........................##################....     |
|                                                                       ....     |
|                                                                                |
----------------------------------------------------------------------------------
Dungeon saved.
```
