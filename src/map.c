#include <stdio.h>
#include <stdlib.h>
#include "map.h"

// границы, пол и случайные препятствия
static void fill_map(Map* m) {
    for (int y = 0; y < m->height; y++) {
        for (int x = 0; x < m->width; x++) {
            if (y == 0 || y == m->height - 1 || x == 0 || x == m->width - 1) {
                m->cells[y][x] = CELL_WALL;
            }
            else {
                m->cells[y][x] = CELL_FLOOR;
            }
        }
    }
    for (int y = 1; y < m->height - 1; y++) {
        for (int x = 1; x < m->width - 1; x++) {
            if (rand() % PERCENT < OBSTACLE_CHANCE) {
                if (rand() % 2 == 0) {
                    m->cells[y][x] = CELL_TREE;
                }
                else {
                    m->cells[y][x] = CELL_ROCK;
                }
            }
            else if (rand() % PERCENT < ICE_CHANCE) {
                m->cells[y][x] = CELL_ICE;
            }
        }
    }
}

// лестница U рядом с героем и лестница D в другом месте
static void place_stairs(Map* m, Player* p) {
    int hx = rand() % (m->width - 3) + 1;
    int hy = rand() % (m->height - 2) + 1;
    m->cells[hy][hx] = CELL_FLOOR;
    m->cells[hy][hx + 1] = CELL_UP;
    p->x = hx;
    p->y = hy;

    int dx, dy;
    do {
        dx = rand() % (m->width - 2) + 1;
        dy = rand() % (m->height - 2) + 1;
    } while (dy == hy && (dx == hx || dx == hx + 1));
    m->cells[dy][dx] = CELL_DOWN;
}
//создаёт карту и выделяет под неё динамическую память
Map* create_map(int floor, Player* p) {
    Map* m = malloc(sizeof(Map));
    if (m == NULL) {
        return NULL;
    }
    m->floor = floor;
    m->height = rand() % MAP_SIZE_RANGE + MAP_MIN_SIZE;
    m->width = rand() % MAP_SIZE_RANGE + MAP_MIN_SIZE;

    m->cells = malloc(m->height * sizeof(char*));
    if (m->cells == NULL) {
        free(m);
        return NULL;
    }
    for (int i = 0; i < m->height; i++) {
        m->cells[i] = malloc(m->width * sizeof(char));
        if (m->cells[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(m->cells[j]);
            }
            free(m->cells);
            free(m);
            return NULL;
        }
    }
    fill_map(m); //заполняет карту стенами, полом и случайными препятствиями
    place_stairs(m, p); //размещает лестницы и устанавливает начальную позицию игрока
    return m;
}
//освобождает всю динамическую память, связанную с картой
void destroy_map(Map* m) {
    if (m == NULL) {
        return;
    }
    for (int i = 0; i < m->height; i++) {
        free(m->cells[i]);
    }
    free(m->cells);
    printf("Память структуры по адресу %p освобождена\n", (void*)m);
    free(m);
}

void print_map(const Map* m, int px, int py) {
    for (int y = 0; y < m->height; y++) {
        for (int x = 0; x < m->width; x++) {
            if (x == px && y == py) {
                printf("@ ");
            }
            else {
                printf("%c ", m->cells[y][x]);
            }
        }
        printf("\n");
    }
}

// функция для проверки, является ли клетка непроходимой
// static это тип функция доступна только внутри данного файла
static int is_blocked(const Map* m, int x, int y) {
    // получаем символ клетки по указанным координатам
    char c = m->cells[y][x];
    return c == CELL_WALL || c == CELL_TREE || c == CELL_ROCK;
}

// функция для перемещения игрока по карте
// принимает указатель на карту, указатель на структуру игрока
int move_player(const Map* m, Player* p, char direction) {
    //переменные для хранения изменения координат игрока
    int dx = 0, dy = 0;
    // в зависимости от введённой клавиши изменяем dx или dy.
    switch (direction) {
    case 'w': case 'W':
        dy = -1;
        break;
    case 's': case 'S':
        dy = 1;
        break;
    case 'a': case 'A':
        dx = -1;
        break;
    case 'd': case 'D':
        dx = 1;
        break;
    default:
        return 0;
    }

    // Проверяем клетку, в которую собирается переместиться игрок для этого к текущим координатам прибавляем dx и dy
    if (is_blocked(m, p->x + dx, p->y + dy)) {
        return 0;
    }
    // Если путь свободен, изменяем координату X игрока.
    p->x += dx;
    // Изменяем координату Y игрока.
    p->y += dy;

    // Проверяем, находится ли игрок на льду. Да --> скользит пока находиться на льду
    while (m->cells[p->y][p->x] == CELL_ICE &&
        !is_blocked(m, p->x + dx, p->y + dy)) {
        p->x += dx;
        p->y += dy;
    }
    return 1;
}