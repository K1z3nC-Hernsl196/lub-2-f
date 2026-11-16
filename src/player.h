#ifndef PLAYER_H
#define PLAYER_H
#include "utils.h"   // нужен для типа GameTime
#define START_HP 100
#define START_STAMINA 50
#define START_GOLD 0
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

Player* create_player(const char* name);
void destroy_player(Player* p);
void print_player(const Player* p);
void sleep_player(Player* p, GameTime* t);   // только прототип, без тела!
#endif