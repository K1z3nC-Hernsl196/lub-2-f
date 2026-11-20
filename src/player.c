#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "player.h"
#include "utils.h"

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
    backpack_init(&p->backpack);
    return p;
}

void destroy_player(Player* p) {
    if (p == NULL) {
        return;
    }
    printf("Память структуры по адресу %p освобождена\n", (void*)p);
    free(p);
}

void print_player(const Player* p, const ItemCatalog* c) {
    printf("\n=== КАРТОЧКА ПЕРСОНАЖА ===\n");
    printf("Имя: %s\n", p->name);
    printf("HP: %d/%d\n", p->hp, p->max_hp);
    printf("Выносливость: %d/%d\n", p->stamina, p->max_stamina);
    printf("Золото: %d\n", p->gold);
    printf("Рюкзак:\n");
    backpack_print(&p->backpack, c);
}

void sleep_player(Player* p, GameTime* t) {
    t->day++;
    t->hour = MORNING_HOUR;
    p->hp = p->max_hp;
    p->stamina = p->max_stamina;
    printf("Вы выспались. Новый день: %d, %02d:00. HP и выносливость восстановлены.\n", t->day, t->hour);
}

// покупка подсумка
void buy_pouch(Player* p) {

    if (p->gold < POUCH_COST) {
        printf("Не хватает золота: подсумок стоит %d, у вас %d.\n",
            POUCH_COST, p->gold);
        return;
    }
    p->gold -= POUCH_COST;

    backpack_add_capacity(&p->backpack, POUCH_CAPACITY);

    printf("Подсумок куплен! Грузоподъёмность: %d. Золото: %d.\n",
        p->backpack.max_capacity, p->gold);
}
