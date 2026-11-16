#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "player.h"

Player* create_player(const char* name) { //выделяем память под Player и возвр. его адрес
    Player* p = malloc(sizeof(Player));
    if (p == NULL) { //проверка на выделение памяти
        return NULL; 
    }
    strncpy(p->name, name, MAX_NAME_LEN - 1);
    p->name[MAX_NAME_LEN - 1] = '\0'; //в последнюю клетку ложим символ конца строки
    p->hp = START_HP;
    p->max_hp = START_HP;
    p->stamina = START_STAMINA;
    p->max_stamina = START_STAMINA;
    p->gold = START_GOLD;
    p->x = 0;
    p->y = 0;
    for (int i = 0; i < BACKPACK_SLOTS; i++) {
        p->inventory[i] = 0;
    }
    return p;
}

void destroy_player(Player* p) {
    if (p == NULL) {
        return;
    }
    printf("Память структуры освобождена\n");
    free(p);
}