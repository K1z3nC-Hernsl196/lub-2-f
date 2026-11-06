#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>

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


// все кейсы в функциях
// 1 case время
void show_time() {
    printf("Текущее время: День %d, %02d:00\n", current_day, current_hour);
}

// 2 case перемоать время
void wait_hours() {
    int hours;
    printf("Сколько часов вы хотите потратить на работу? ");
    if (scanf("%d", &hours) != 1) {
        printf("Какие буквы? Только числа.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    if (hours < 0) {
        printf("Количество часов не может быть отрицательным!\n");
        return;
    }
    current_hour += hours;
    while (current_hour >= 24) {
        current_hour -= 24;
        current_day++;
    }
    printf("Новое время: День %d, %02d:00\n", current_day, current_hour);
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
        printf("Только числа!\n"); while (getchar() != '\n'); return;}
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

// фун-я очистки остатков ввода
void clear_input(void) { 
    int c;

    while ((c = getchar()) != '\n' && c != EOF) { //проверяет что это не конец строки И не конец входного потока
    }                                             // return ненужен т.к. зач вообще нужны уловия тут??
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

    clear_input();

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


// Создание карты
char** create_map(int height, int width) {

    // Выделяем память под указатели на строки
    // map будет хранить адрес участка памяти, в котором находятся указатели на строки карты 
    char** map = malloc(height * sizeof(char*));

    if (map == NULL) {
        return NULL;
    }

    // выделяем память под каждую строку
    for (int i = 0; i < height; i++) {

        map[i] = malloc(width * sizeof(char));

        if (map[i] == NULL) {

            // освобождаем уже созданные строки если че то пошло нетак
            for (int j = 0; j < i; j++) {
                free(map[j]); // освобождаем память дивианта
            }

            free(map); // и массив указателей
            return NULL;
        }
    }

    // границы и внутр. часть
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            if (y == 0 || y == height - 1 ||
                x == 0 || x == width - 1) {
//клетка в первой\последней строке И клетка в первом\последнем столбце
                map[y][x] = '#';
            }
            else {
                map[y][x] = '.';
            }
        }
    }

    // препятствия
    for (int y = 1; y < height - 1; y++) { 
        for (int x = 1; x < width - 1; x++) {
            // заполняем внутренности
            if (rand() % 100 < 20) {
                //остаток от деления на 100, то есть число от 0 до 99.
                // < 20 потому что препятствие появится при числах от 0 до 19.(20%)
                if (rand() % 2 == 0) {
                    map[y][x] = 'T';
                }
                else {
                    map[y][x] = 'R';
                }

            }
            else if (rand() % 100 < 30) {
                map[y][x] = '~';
            }
        }
    }

    return map;
}


// выводжим карту
void print_map(char** map, int height, int width) {

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            printf("%c ", map[y][x]);
        }

        printf("\n");
    }
}


// освобождение памяти
void free_map(char** map, int height) {

    for (int i = 0; i < height; i++) {
        free(map[i]);
    }

    free(map);
}


// подземелье
void dungeon_map(void) {
    //случайные размеры от 5*5 до 30*30
    int height = rand() % 26 + 5;
    int width = rand() % 26 + 5;

    printf("\n=== ПОДЗЕМЕЛЬЕ ===\n");
    printf("Размер карты: %d x %d\n", height, width);
    char** map = create_map(height, width);

    if (map == NULL) {
        printf("Не удалось создать карту!\n");
        return;
    }

    int player_x = rand() % (width - 2) + 1;
    int player_y = rand() % (height - 2) + 1;

    while (map[player_y][player_x] != '.') {
        player_x = rand() % (width - 2) + 1;
        player_y = rand() % (height - 2) + 1;
    }

    map[player_y][player_x] = '@';

    print_map(map, height, width);

    char direction;

    while (1) {

        print_map(map, height, width);

        printf("\nW/A/S/D - движение, Q - выход: ");

        if (scanf(" %c", &direction) != 1) {
            clear_input();
            continue;
        }

        clear_input();

        if (direction == 'Q' || direction == 'q') {
            break;
        }

        if (move_player(map, height, width,
            &player_x, &player_y, direction) == 0) {

            printf("Туда пройти нельзя!\n");
        }
    }

    free_map(map, height);

}
// движение wasd
int move_player(char** map, int height, int weight, int* player_x, int* player_y, char direction){
    int new_x = *player_x;
    int new_y = *player_y;

    if (direction == 'W' || direction == 'w') {
        new_y--;
    }
    else if (direction == 'S' || direction == 's') {
        new_y++;
    }
    else if (direction == 'A' || direction == 'a') {
        new_x--;
    }
    else if (direction == 'D' || direction == 'd') {
        new_x++;
    }
    //проверяем границы краты
    if (map[new_y][new_x] == '#' || map[new_y][new_x] == 'T' || map[new_y][new_x] == 'R') {
        return 0;
    }
    // убираем героя со старой клетки
    map[*player_y][*player_x] = '.';

    // обновляем координаты
    *player_x = new_x;
    *player_y = new_y;

    // ставим героя на новую клетку
    map[*player_y][*player_x] = '@';

    return 1;
}
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
   
    if (load_items(item_names) == 0) {
        printf("items.txt не найден. Используем стандартные названия.\n");
        set_default_items(item_names);
    }
    printf("Введите имя фермера: ");
    if (fgets(farmer_name, sizeof(farmer_name), stdin) != NULL) {
        farmer_name[strcspn(farmer_name, "\n")] = '\0';

        if (strlen(farmer_name) == 0) {
            strcpy(farmer_name, "Фермер");
        }
    }
    int running = 1;
    int choice;

    while (running)
    {
        show_menu();

        if (scanf("%d", &choice) != 1) {
            printf("Какие буквы? Только числа.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (choice)
        {
        case 0: 
            running = 0; 
            break;
        case 1: 
            show_time(); break;
        case 2: 
            wait_hours(); break;
        case 3: 
            show_inventory();break;
        case 4: 
            put_item(); break;
        case 5: 
            discard_item(); break;
        case 6: 
            remove_duplicates(); break;
        case 7:
            search_item(); break;
        case 8:
            save_diary(); break;
        case 9:
            nps_dialogs(); break;
        case 10:
            dungeon_map(); break;
        default:
            printf("Такого пункта в меню нет.\n");
            break;
        }
    }
    return 0;
}