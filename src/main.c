#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include "player.h"
#include "utils.h"
#include "backpack.h"
#include "adventure.h"
#include "files.h"

// пункты главного меню
enum MenuItem {
    MENU_EXIT = 0,
    MENU_CREATE,
    MENU_CARD,
    MENU_TIME,
    MENU_INVENTORY,
    MENU_SEARCH,
    MENU_DIARY,
    MENU_DIALOGS,
    MENU_ADVENTURE
};

// пункты подменю времени
enum ClockItem {
    CLK_BACK = 0,
    CLK_SHOW,
    CLK_WAIT,
    CLK_SLEEP
};

// пункты подменю инвентаря
enum InventoryItem {
    INV_BACK = 0,
    INV_SHOW,
    INV_PUT,
    INV_DISCARD,
    INV_DEDUP,
    INV_POUCH
};

#define NAME_PROMPT "Введите имя героя: "

// проверка: герой создан? (используем во всех пунктах, где он нужен)
static int require_player(const Player* p) {
    if (p == NULL) {
        printf("Сначала создайте героя!\n");
        return 0;
    }
    return 1;
}

static void create_or_recreate(Player** player) {
    if (*player != NULL) {
        int sure = read_int("Герой уже есть. Удалить его? (1 - да, 0 - нет): ", 0, 1);
        if (!sure) {
            return;
        }
        destroy_player(*player);
        *player = NULL;
    }
    char name[MAX_NAME_LEN];
    read_line(NAME_PROMPT, name, MAX_NAME_LEN);
    *player = create_player(name);
    if (*player == NULL) {
        printf("Не хватило памяти!\n");
    }
    else {
        printf("Герой создан!\n");
    }
}

static void put_into_backpack(Player* p, const ItemCatalog* catalog) {
    printf("Предметы:\n");
    for (int i = 0; i < ItemCount; i++) {
        printf("  [%d] %s (вес %d)\n", i, item_name(catalog, i), item_weight(i));
    }
    int slot = read_int("Номер слота (0-9): ", 0, BACKPACK_SLOTS - 1);
    int id = read_int("ID предмета: ", 0, ItemCount - 1);
    backpack_put(&p->backpack, slot, id);
    printf("Предмет %s лежит в слоте %d.\n", item_name(catalog, id), slot);
}

static void discard_from_backpack(Player* p) {
    int slot = read_int("Номер слота (0-9): ", 0, BACKPACK_SLOTS - 1);
    backpack_remove(&p->backpack, slot);
    printf("Слот %d очищен.\n", slot);
}

// подменю времени (не засоряем главное меню)
static void time_menu(Player* player, GameTime* game_time) {
    int back = 0;
    while (!back) {
        printf("\n--- ВРЕМЯ ---\n"
            "[1] Посмотреть на часы\n"
            "[2] Ждать\n"
            "[3] Лечь спать\n"
            "[0] Назад\n");
        int choice = read_int("Выберите действие: ", CLK_BACK, CLK_SLEEP);
        switch (choice) {
        case CLK_BACK:
            back = 1;
            break;
        case CLK_SHOW:
            show_time(game_time);
            break;
        case CLK_WAIT:
            wait_time(game_time);
            break;
        case CLK_SLEEP:
            if (require_player(player)) {
                sleep_player(player, game_time);
            }
            break;
        }
    }
}

static void inventory_menu(Player* player, const ItemCatalog* catalog) {
    int back = 0;
    while (!back) {
        printf("\n--- ИНВЕНТАРЬ ---\n"
            "[1] Показать\n"
            "[2] Положить предмет\n"
            "[3] Выбросить предмет\n"
            "[4] Убрать повторы\n"
            "[5] Купить подсумок (%d золота)\n"
            "[0] Назад\n", POUCH_COST);
        int choice = read_int("Выберите действие: ", INV_BACK, INV_POUCH);
        switch (choice) {
        case INV_BACK:
            back = 1;
            break;
        case INV_SHOW:
            backpack_print(&player->backpack, catalog);
            break;
        case INV_PUT:
            put_into_backpack(player, catalog);
            break;
        case INV_DISCARD:
            discard_from_backpack(player);
            break;
        case INV_DEDUP:
            backpack_remove_duplicates(&player->backpack);
            backpack_print(&player->backpack, catalog);
            break;
        case INV_POUCH:
            buy_pouch(player);
            break;
        }
    }
}

int main(void) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    srand((unsigned int)time(NULL));

    ItemCatalog catalog;
    catalog_init(&catalog);   // названия предметов: из items.txt или стандартные

    Player* player = NULL;   // героя пока нет
    GameTime game_time = { START_DAY, MORNING_HOUR };
    int running = 1;

    while (running) {
        printf("\n=== ГЛАВНОЕ МЕНЮ ===\n"
            "[1] Создать героя\n"
            "[2] Карточка\n"
            "[3] Время\n"
            "[4] Инвентарь\n"
            "[5] Поиск предмета\n"
            "[6] Записать в дневник\n"
            "[7] Диалоги НПС\n"
            "[8] Поход\n"
            "[0] Выход\n");
        int choice = read_int("Выберите действие: ", MENU_EXIT, MENU_ADVENTURE);

        switch (choice) {

        case MENU_EXIT:
            running = 0;
            break;

        case MENU_CREATE:
            create_or_recreate(&player);
            break;

        case MENU_CARD:
            if (require_player(player)) {
                print_player(player, &catalog);
                show_time(&game_time);
            }
            break;

        case MENU_TIME:
            time_menu(player, &game_time);
            break;

        case MENU_INVENTORY:
            if (require_player(player)) {
                inventory_menu(player, &catalog);
            }
            break;

        case MENU_SEARCH:
            if (require_player(player)) {
                search_item(player, &catalog);
            }
            break;

        case MENU_DIARY:
            if (require_player(player)) {
                save_diary(player, &game_time, &catalog);
            }
            break;

        case MENU_DIALOGS:
            if (require_player(player)) {
                npc_dialogs(player, &game_time, &catalog);
            }
            break;

        case MENU_ADVENTURE:
            if (require_player(player)) {
                start_adventure(player, &game_time);
            }
            break;
        }
    }
    destroy_player(player);
    return 0;
}