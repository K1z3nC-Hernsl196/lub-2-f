#ifndef MAP_H
#define MAP_H
#include "player.h"

#define MAP_MIN_SIZE     5
#define MAP_SIZE_RANGE   26
#define OBSTACLE_CHANCE  20
#define ICE_CHANCE       15
#define PERCENT          100

// типы клеток карты (значение = символ)
typedef enum {
    CELL_WALL = '#',
    CELL_FLOOR = '.',
    CELL_TREE = 'T',
    CELL_ROCK = 'R',
    CELL_ICE = '~',
    CELL_DOWN = 'D',
    CELL_UP = 'U'
} CellType;

typedef struct {
    char** cells;   // двумерный динамический массив
    int width;
    int height;
    int floor;      // номер этажа
} Map;

Map* create_map(int floor, Player* p);
void destroy_map(Map* m);
void print_map(const Map* m, int px, int py);

#endif