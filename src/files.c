#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "files.h"
#define DIARY_FILE    "diary.txt"   // файл дневника
#define INPUT_FILE    "input.txt"   // диавлоги NPS
#define OUTPUT_FILE   "output.txt"  // запись диалогов
#define LINE_BUF      1024          // максимальный размер буфера для чтения одной строки из файла
#define DAY_TEXT_LEN  16            // размер массива для хранения номера игрового дня в виде строки
#define TAG_NAME      "<NAME>"      //теги диалогов
#define TAG_DAY       "<DAY>"
#define TAG_ITEM      "<ITEM>"
#define DIARY_LINE    "~~~~~~~~~~~~~~~~~~~~~~"


// поиск предмета по названию
void search_item(const Player* p, const ItemCatalog* c) {
    char search_name[ITEM_NAME_LEN];
    int found = 0;
    // read_line() считывает строку и сохраняет её в search_name.
    read_line("Введите название предмета: ", search_name, ITEM_NAME_LEN);

    for (int id = 0; id < ItemCount; id++) {
        // сравниваем название текущего предмета с введённым пользователем
        if (strcmp(item_name(c, id), search_name) != 0) {
            continue;
        }
        found = 1;
        printf("Предмет найден: ID %d - %s\n", id, item_name(c, id));

        // изначально считаем, что предмета в рюкзаке нет
        int in_bag = 0;
        for (int s = 0; s < BACKPACK_SLOTS; s++) {
            if (p->backpack.slots[s] == id) {
                printf("  лежит в слоте %d\n", s);
                in_bag = 1;
            }
        }

        if (!in_bag) {
            printf("  в рюкзаке этого предмета нет\n");
        }
    }
    if (!found) {
        printf("Предмет с названием \"%s\" не найден\n", search_name);
    }
}


// запись состояния игры в дневник
void save_diary(const Player* p, const GameTime* t, const ItemCatalog* c) {

    FILE* file = fopen(DIARY_FILE, "a");
    if (file == NULL) {
        printf("Не удалось открыть %s\n", DIARY_FILE);
        return;
    }

    printf("\n--- Состояние игры ---\n");
    fprintf(file, "--- Состояние игры ---\n");

    printf("Фермер: %s\n", p->name);
    fprintf(file, "Фермер: %s\n", p->name);

    printf("День: %d, Время: %02d:00\n", t->day, t->hour);
    fprintf(file, "День: %d, Время: %02d:00\n", t->day, t->hour);

    printf("Рюкзак:\n");
    fprintf(file, "Рюкзак:\n");

    for (int i = 0; i < BACKPACK_SLOTS; i++) {

        printf("Слот %d: %s\n", i, item_name(c, p->backpack.slots[i]));
        fprintf(file, "Слот %d: %s\n", i, item_name(c, p->backpack.slots[i]));
    }

    printf("%s\n", DIARY_LINE);
    fprintf(file, "%s\n", DIARY_LINE);

    fclose(file);

    printf("Состояние игры записано в %s\n", DIARY_FILE);
}


// проверка тегов
static size_t tag_len(const char* s, const char* tag) {
    size_t len = strlen(tag);            // вычисляем длину тега
    if (strncmp(s, tag, len) == 0) {     // сравниваем начало строки s с тегом

        return len;
    }

    return 0;
}

static void emit(FILE* out, const char* text) {

    fprintf(out, "%s", text);    // записываем текст в файл
    printf("%s", text);          // выводим
}


// обработка диалогов NPC
void npc_dialogs(const Player* p, const GameTime* t, const ItemCatalog* c) {

    FILE* input = fopen(INPUT_FILE, "r");
    if (input == NULL) {
        printf("Не удалось открыть %s\n", INPUT_FILE);
        return;
    }

    FILE* output = fopen(OUTPUT_FILE, "w"); //открываем в режие записи
    if (output == NULL) {
        printf("Не удалось открыть %s\n", OUTPUT_FILE);
        fclose(input);
        return;
    }


    char line[LINE_BUF];            //для хранения строки, прочитанной из input.txt.
    char day_text[DAY_TEXT_LEN];    //для хранения номера игрового дня 

    snprintf(day_text, sizeof(day_text), "%d", t->day); // число дня -> текст
    printf("\n--- NPC Диалог ---\n");

    // читаем файл построчно
    while (fgets(line, sizeof(line), input) != NULL) {

        const char* s = line; // s показывает на текущий символ строки
        while (*s != '\0') {

            size_t len_name = tag_len(s, TAG_NAME);
            size_t len_day = tag_len(s, TAG_DAY);
            size_t len_item = tag_len(s, TAG_ITEM);
            if (len_name > 0) {
                emit(output, p->name);
                s += len_name;  // перепрыгиваем через тег
            }
            else if (len_day > 0) {
                emit(output, day_text);
                s += len_day;
            }
            else if (len_item > 0) {
                emit(output, item_name(c, p->backpack.slots[0]));
                s += len_item;
            }
            else {
                fputc(*s, output); // обычный символ - копируем как есть
                putchar(*s);
                s++;
            }
        }
    }

    fclose(input);
    fclose(output);
    printf("\n==Диалог сохранён в %s==\n", OUTPUT_FILE);
}