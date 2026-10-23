#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <string.h>

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
void save_diary(void) {
    FILE* file = fopen("diary.txt", "a");

    if (file == NULL) {
        printf("Не удалось открыть diary.txt\n");
        return;
    }

    fprintf(file, "Farmer: %s\n", farmer_name);
    fprintf(file, "Day: %d, Time: %02d:00\n", current_day, current_hour);
    fprintf(file, "Inventory:\n");

    for (int i = 0; i < INV_SIZE; i++) {
        fprintf(file, "Slot %d: %s\n", i, item_names[inventory[i]]);
    }                                  
    fprintf(file, "~~~~~~~~~~~~~~~~~~~~~\n");

    fclose(file);

    printf("Состояние игры записано в diary.txt\n");
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
   
    if (load_items(item_names) == 0) {
        printf("items.txt не найден. Используем стандартные названия.\n");
        set_default_items(item_names);
    }
    printf("Проверка ебучих названий: %s, %s, %s\n", item_names[0], item_names[1], item_names[2]);
    printf("Введите имя фермера: ");
    if (scanf("%32s", farmer_name) != 1) {
        strcpy(farmer_name, "Фермер");
    }
    while (getchar() != '\n');

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
        default:
            printf("Такого пункта в меню нет.\n");
            break;
        }
    }
    return 0;
}