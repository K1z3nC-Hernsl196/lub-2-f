#ifndef PLAYER_H
#define PLAYER_H

#define MAX_NAME_LEN    32
#define BACKPACK_SLOTS  10

typedef struct {
    char name[MAX_NAME_LEN];
    int hp;
    int max_hp;
    int stamina;
    int max_stamina;
    int gold;
    int x, y;
    int inventory[BACKPACK_SLOTS];
} Player;

#endif
