#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "player.h"
#include "utils.h"
#include "map.h"
#include "adventure.h"

#define INV_SIZE 10
#define MAX_ITEMS 10
#define MAX_NAME_LEN 32
#define MAX_FARMER_NAME 33


// глобальные переменные (видны везде)
char farmer_name[MAX_FARMER_NAME] = "Фермер";
int current_day = 1;
int current_hour = 8;
int inventory[INV_SIZE] = { 0 };
char item_names[MAX_ITEMS][MAX_NAME_LEN];

int load_items(char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    FILE* file = fopen("items.txt", "r");
    if (file == NULL) {
        return 0;
    }
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (fgets(item_names[i], MAX_NAME_LEN, file) == NULL) {
            fclose(file);
            return 0;
        }     //помогает найти позиию симв. в строке
        item_names[i][strcspn(item_names[i], "\n")] = '\0'; // убираем перевод строки т.к храним только название предмета
    }
    fclose(file);
    return 1;
}

void set_default_items(char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    const char* defaults[MAX_ITEMS] = {
        "Empty", "Wood", "Stone", "Seeds", "Carrot",
        "Water Bucket", "Soil", "Fish", "Fishing Rod", "Shovel"
    };
    for (int i = 0; i < MAX_ITEMS; i++) {
        strcpy(item_names[i], defaults[i]);
    }
}

void show_menu(void) {
    printf("\n===== МЕНЮ ФЕРМЕРА %s ====\n"
        "[0] Выход\n"
        "[1] Посмотреть на часы\n"
        "[2] Перемотать время(поработать)\n"
        "[3] Посмотреть инвентарь\n"
        "[4] Положить предмет в слот\n"
        "[5] Выбросить предмет\n"
        "[6] Удалить повторы из инвентаря\n"
        "[7] Поиск предмета по названию\n"
        "[8] Записать состояние в дневник\n"
        "[9] Обработать дилогов НПС\n"
        "[10] Отправиться в приключение\n"
        "Выберите действие: ", farmer_name);
}


// 3 case инвентарь
void show_inventory() {
    for (int i = 0; i < INV_SIZE; i++) {
        printf("Слот %d: [%d] %s\n", i, inventory[i], item_names[inventory[i]]);
    }
}

// 4 case положить предм в инв.
void put_item() {
    int slot, item_id;
    printf("Введите номер слота(0-9): ");
    if (scanf("%d", &slot) != 1) {
        printf("Только числа!\n"); while (getchar() != '\n'); return;
    }
    if (slot < 0 || slot >= INV_SIZE) { printf("Такого слота нет\n"); return; }

    printf("Введите ID предмета (0-9): ");
    if (scanf("%d", &item_id) != 1) { printf("Только числа!\n"); while (getchar() != '\n'); return; }
    while (getchar() != '\n');

    if (item_id < 0 || item_id >= MAX_ITEMS) { printf("Такого ID нет\n"); return; }

    inventory[slot] = item_id;
    printf("Предмет помещен в слот %d\n", slot);
}

// 5 case выбросить предмет
void discard_item() {
    int slot;
    printf("Введите номер слота(0-9): ");
    if (scanf("%d", &slot) != 1) { printf("Только числа!\n"); while (getchar() != '\n'); return; }
    while (getchar() != '\n');

    if (slot < 0 || slot >= INV_SIZE) { printf("Такого слота нет\n"); return; }

    inventory[slot] = 0;
    printf("Предмет выброшен из слота %d\n", slot);
}

//6 case убрать дубликаты
void remove_duplicates(void) {
    show_inventory();

    for (int i = 0; i < INV_SIZE; i++) {
        for (int j = i + 1; j < INV_SIZE; j++) {
            if (inventory[j] == inventory[i] && inventory[i] != 0) {
                inventory[j] = 0;
            }
        }
    }

    printf("Измененный инвентарь:\n");
    show_inventory();
}

//[7]
void search_item(void) {
    char search_name[MAX_NAME_LEN];
    int found = 0;
    printf("Введите название предмета: ");
    // читаем назв целиком + пробелы
    if (scanf(" %31[^\n]", search_name) != 1) {
        printf("Не удалось прочитать название.\n");
        clear_input();
        return;
    }

    for (int i = 0; i < MAX_ITEMS; i++) {
        if (strcmp(item_names[i], search_name) == 0) {
            printf("Предмет найден: ID %d - %s\n", i, item_names[i]);
            found = 1;
        }
    }
    if (found == 0) {
        printf("Предмет с названием \"%s\" не найден \n", search_name);
    }
}
//[8]
void save_diary(void) {
    FILE* file = fopen("diary.txt", "a");

    if (file == NULL) {
        printf("Не удалось открыть diary.txt\n");
        return;
    }

    printf("\n--- Состояние игры ---\n");
    fprintf(file, "--- Состояние игры ---\n");

    printf("Фермер: %s\n", farmer_name);
    fprintf(file, "Фермер: %s\n", farmer_name);

    printf("День: %d, Время: %02d:00\n", current_day, current_hour);
    fprintf(file, "ДЕнь: %d, Время: %02d:00\n", current_day, current_hour);

    printf("Инвентарь:\n");
    fprintf(file, "Инвентарь:\n");

    for (int i = 0; i < INV_SIZE; i++) {
        printf("Слот %d: %s\n", i, item_names[inventory[i]]);
        fprintf(file, "Слот %d: %s\n", i, item_names[inventory[i]]);
    }

    printf("~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~\n");

    fclose(file);

    printf("Состояние игры записано в diary.txt\n");
}
//[9]
void nps_dialogs(void) {
    FILE* input = fopen("input.txt", "r");

    if (input == NULL) {
        printf("Не удалось открыть input.txt\n");
        return;
    }
    // write режим записи
    FILE* output = fopen("output.txt", "w"); // output  Готовый персонализированный текст письма по заданию

    if (output == NULL) {
        printf("Не удалось открыть output.txt\n");
        fclose(input);
        return;
    }
    // сhar хранит симв.
    char line[1024]; // массивчик для считывания текста из файла
    char day_text[16];// масив для номера для в виде строки т.к. current_day целое число а нужен текст
    //куда запис, размер буфера, итд крч
    snprintf(day_text, sizeof(day_text), "%d", current_day); // преобраз числор в текст

    printf("\n--- NPC Диалог ---\n");
    // цикл который работает пока удаеться читать строки или ее части
            //просто масив, макс. размер считыв. данных и откуда читаем
    while (fgets(line, sizeof(line), input) != NULL) {
        const char* p = line;
        // ^ просто указать на  текущ. позицию в строке
        while (*p != '\0') {
            if (strncmp(p, "<NAME>", 6) == 0) { // если текст начинается с NAME пишет имя фер. если нет копирует тек. симв как есть
                fprintf(output, "%s", farmer_name);
                printf("%s", farmer_name);
                p += 6; // перемещаем указатель на 6 симв. ЗА шаблон т.к. программа начнет обрабатывать NAME повторно
            }
            else if (strncmp(p, "<DAY>", 5) == 0) {
                fprintf(output, "%s", day_text); // номер тек. дня в виде строки 
                printf("%s", day_text);
                p += 5; // тож самое, пропускает найденый шаблон
            }
            else if (strncmp(p, "<ITEM>", 6) == 0) {
                fprintf(output, "%s", item_names[inventory[0]]);
                printf("%s", item_names[inventory[0]]);
                p += 6;
            } // все тоже самое как и выше
            else {
                fputc(*p, output); // записываем тек.сим в файл
                putchar(*p); // выводим его на экран
                p++; // передвигаем на следущ симв
            }
        }
    }
    // ну и закрыть файлы
    fclose(input);
    fclose(output);

    printf("\n==Диалог сохранён в output.txt==\n");
}


enum MenuItem {
    MENU_EXIT = 0,
    MENU_CREATE,
    MENU_CARD,
    MENU_TIME,
    MENU_WAIT,
    MENU_SLEEP,
    MENU_ADVENTURE
};

#define NAME_PROMPT "Введите имя героя: "

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

int main(void) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    srand((unsigned int)time(NULL));

    Player* player = NULL;
    GameTime game_time = { START_DAY, MORNING_HOUR };   //время игры
    int running = 1;

    while (running) {
     
        printf("\n[1] Создать героя\n[2] Карточка\n[3] Часы\n[4] Ждать\n[5] Спать\n[6] Поход\n[0] Выход\n");
        int choice = read_int("Выберите действие: ", MENU_EXIT, MENU_ADVENTURE);

        switch (choice) {

        case MENU_EXIT:
            running = 0;
            break;

        case MENU_CREATE:
            create_or_recreate(&player);
            break;

        case MENU_CARD:
            if (player == NULL) {
                printf("Сначала создайте героя!\n");
            }
            else {
                print_player(player);
                show_time(&game_time); 
            }
            break;

        case MENU_TIME:
            show_time(&game_time);
            break;

        case MENU_WAIT:
            wait_time(&game_time);
            break;

        case MENU_SLEEP:
            if (player == NULL) {
                printf("Сначала создайте героя!\n");
            }
            else {
                sleep_player(player, &game_time);
            }
            break;

        case MENU_ADVENTURE:
            if (player == NULL) {
                printf("Сначала создайте героя!\n");
            }
            else {
                start_adventure(player, &game_time);
            }
            break;
        }
    }
    destroy_player(player);
    return 0;
}
//int main()
//{   
//    srand((unsigned int)time(NULL));
//    SetConsoleOutputCP(CP_UTF8);
//    SetConsoleCP(CP_UTF8);
////тест
//    Player* player = NULL;
//player = create_player("Тест");
//if (player == NULL) {
//    printf("Не хватило памяти\n");
//    return 1;
//}
//printf("%s: золото %d, HP %d/%d\n", player->name, player->gold, player->hp, player->max_hp);
//destroy_player(player);
//player = NULL;
//
//
//    if (load_items(item_names) == 0) {
//        printf("items.txt не найден. Используем стандартные названия.\n");
//        set_default_items(item_names);
//    }
//    printf("Введите имя фермера: ");
//    if (fgets(farmer_name, sizeof(farmer_name), stdin) != NULL) {
//        farmer_name[strcspn(farmer_name, "\n")] = '\0';
//
//        if (strlen(farmer_name) == 0) {
//            strcpy(farmer_name, "Фермер");
//        }
//    }
//    int running = 1;
//    int choice;
//
//    while (running)
//    {
//        show_menu();
//
//        if (scanf("%d", &choice) != 1) {
//            printf("Какие буквы? Только числа.\n");
//            while (getchar() != '\n');
//            continue;
//        }
//        while (getchar() != '\n');
//
//        switch (choice)
//        {
//        case 0: 
//            running = 0; 
//            break;
//        case 1: 
//            show_time(); break;
//        case 2: 
//            wait_hours(); break;
//        case 3: 
//            show_inventory();break;
//        case 4: 
//            put_item(); break;
//        case 5: 
//            discard_item(); break;
//        case 6: 
//            remove_duplicates(); break;
//        case 7:
//            search_item(); break;
//        case 8:
//            save_diary(); break;
//        case 9:
//            nps_dialogs(); break;
//        case 10:
//            dungeon_map(); break;
//        default:
//            printf("Такого пункта в меню нет.\n");
//            break;
//        }
//    }
//    return 0;
//}