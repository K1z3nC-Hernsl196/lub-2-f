#include <stdio.h>      
#include "adventure.h"  
#include "map.h"        

#define STEP_STAMINA_COST   1    // шаг по карте
#define FLOOR_STAMINA_COST  5    // переход на новый этаж
#define FLOOR_TIME_COST     1    // часов на переход
#define GOLD_BASE_REWARD    10   // базовый множитель награды
#define GOLD_LOSS_DIVIDER   2    // при обмороке теряем 1/2 золота


// функция проверки возможности начать приключение
static int can_start(const Player* p, const GameTime* t) {
    //проверем выносливость и не наступила ли носчь
    if (p->stamina <= 0) {
        printf("Больше нет сил идти. Нужно поспать!\n");
        return 0;
    }
    if (hours_until_night(t) == 0) {
        printf("Ночью слишком опасно. Нужно поспать!\n");
        return 0;
    }
    return 1;
}


// функция для вывода текущего состояния игрока и игрового времени
// const только считывает данные, но не меняет их
static void print_hud(const Player* p, const Map* m, const GameTime* t) {
    printf("\nHP: %d/%d | Выносливость: %d/%d | Золото: %d | Этаж: %d | Время: %02d:00\n",
        p->hp,
        p->max_hp,
        p->stamina,
        p->max_stamina,
        p->gold,
        m->floor,
        t->hour);
}


// функция для уменьшения выносливости игрока
// принимает указатель Player и количество выносливости которое нужно потратить
static void spend_stamina(Player* p, int amount) {
    p->stamina -= amount;
    if (p->stamina < 0) {
        p->stamina = 0;
    }
}

// функция проверки: закончились силы или наступила ночь
static int is_exhausted(const Player* p, const GameTime* t) {
    return p->stamina <= 0 || hours_until_night(t) == 0;
}


// функция обработки обморока игрока она принимает указатель на структуру игрока
static void faint(Player* p) {
    p->gold -= p->gold / GOLD_LOSS_DIVIDER;
    printf("\nГерой падает без сил! Спасатели помогли, но взяли кое что взамен..\n");
    printf("Золото теперь: %d\n", p->gold);
}


// функция перехода на следующий этаж подземелья
// принимает указатель на указатель карты, указатель на игрока и указатель на игровое время.
static int descend(Map** map, Player* p, GameTime* t) {
    int next_floor = (*map)->floor + 1;

    // освобождаем память старой карты
    destroy_map(*map);

    // Создаём новую карту с номером следующего этажа
    *map = create_map(next_floor, p);
    if (*map == NULL) {
        return 0;
    }

    //списываем выносливость за переход на следующий этаж
    spend_stamina(p, FLOOR_STAMINA_COST);

    //увеличиваем игровое время на один час
    advance_time(t, FLOOR_TIME_COST);
    p->gold += GOLD_BASE_REWARD * next_floor;
    printf("Вы спустились на этаж %d! Награда: %d золота.\n",
        next_floor,
        GOLD_BASE_REWARD * next_floor);

    return 1;
}


// основная функция запуска приключения
void start_adventure(Player* player, GameTime* time) {
    // сначала проверяем, может ли игрок отправиться в приключение.
    if (!can_start(player, time)) {
        return;
    }
    // Создаём первую карту подземелья
    Map* map = create_map(1, player);
    if (map == NULL) {
        printf("Не удалось создать карту!\n");
        return;
    }

    printf("\n=== ПОДЗЕМЕЛЬЕ ===\n");
    while (1) {
        print_hud(player, map, time);
        print_map(map, player->x, player->y);
        printf("\nW/A/S/D - движение, Q - домой (только на лестнице U): ");
        //переменная для хранения нажатой пользователем клавиши
        char direction;
        // считываем один символ с клавиатуры
        int r = scanf(" %c", &direction);
        // проверяем, не достигнут ли конец входного потока
        if (r == EOF) {
            break;
        }
        clear_input();

        // если ввод какой то нетакой, начинаем следующую итерацию цикла
        if (r != 1) {
            continue;
        }

        if (direction == 'Q' || direction == 'q') {
            if (map->cells[player->y][player->x] == CELL_UP) {
                printf("Вы вернулись домой с золотом: %d\n", player->gold);
                break;
            }
            printf("Домой можно уйти только с лестницы U!\n");
            continue;
        }

        if (!move_player(map, player, direction)) {
            printf("Туда пройти нельзя!\n");
            continue;
        }

        // если игрок успешно переместился, уменьшаем его выносливость на стоимость одного шага
        spend_stamina(player, STEP_STAMINA_COST);

        if (map->cells[player->y][player->x] == CELL_DOWN) {

            // если игрок наступил на лестницу вниз, вызываем функцию перехода на следующий этаж
            if (!descend(&map, player, time)) {
                printf("Не удалось создать этаж!\n");
                return;
            }
        }

        // если игрок устал или наступила ночь вызываем функцию обработки обморока
        if (is_exhausted(player, time)) {
            faint(player);
            break;
        }
    }
    //освобождаем память карты
    destroy_map(map);
}