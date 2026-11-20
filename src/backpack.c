#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "backpack.h"

// веса предметов (константа, менять её нельзя)
static const int weights[ItemCount] = {
    [Empty] = WEIGHT_EMPTY,
    [Wood] = WEIGHT_WOOD,
    [Stone] = WEIGHT_STONE,
    [Seeds] = WEIGHT_SEEDS,
    [Carrot] = WEIGHT_CARROT,
    [WaterBucket] = WEIGHT_WATER_BUCKET,
    [Soil] = WEIGHT_SOIL,
    [Fish] = WEIGHT_FISH,
    [FishingRod] = WEIGHT_FISHING_ROD,
    [Shovel] = WEIGHT_SHOVEL
};

//определения веса предмета по его идентификатору
int item_weight(int item_id) {
    // проверяем, входит ли идентификатор предмета в допустимый диапазон
    if (item_id < 0 || item_id >= ItemCount) {
        return 0;
    }
    return weights[item_id];
}


// читает названия из файла: 1 - успех, 0 - файла нет или он неполный
static int load_names_from_file(ItemCatalog* c) {
    FILE* f = fopen(ITEMS_FILE, "r");
    if (f == NULL) {
        return 0;
    }
    for (int i = 0; i < ItemCount; i++) {
        if (fgets(c->names[i], ITEM_NAME_LEN, f) == NULL) {
            fclose(f);
            return 0;
        }
        // убираем перевод строки, храним только название
        c->names[i][strcspn(c->names[i], "\r\n")] = '\0';
        if (c->names[i][0] == '\0') {
            fclose(f);
            return 0;
        }
    }
    fclose(f);
    return 1;
}


// стандартные названия, если файла нет
static void set_default_names(ItemCatalog* c) {
    static const char* const defaults[ItemCount] = {
        "Empty", "Wood", "Stone", "Seeds", "Carrot",
        "Water Bucket", "Soil", "Fish", "Fishing Rod", "Shovel"
    };
    for (int i = 0; i < ItemCount; i++) {
        strcpy(c->names[i], defaults[i]);
    }
}


// заполняет каталог: из файла, а если не вышло - стандартными названиями
void catalog_init(ItemCatalog* c) {
    if (!load_names_from_file(c)) {
        printf("%s не найден или повреждён. Используем стандартные названия.\n", ITEMS_FILE);
        set_default_names(c);
    }
}


// функция для получения названия предмета по его идентификатору
// (возвращает указатель на строку с названием предмета)
const char* item_name(const ItemCatalog* c, int item_id) {
    if (item_id < 0 || item_id >= ItemCount) {
        return "???";
    }
    return c->names[item_id];
}


// функция для получения текстового названия состояния загрузки рюкзака
const char* load_state_name(LoadState s) {
    switch (s) {
    case LOAD_LIGHT:
        return "Налегке";
    case LOAD_NORMAL:
        return "Норма";
    case LOAD_OVERLOAD:
        return "Перегруз";
    }
    return "?";
}


//пересчёт общего веса предметов и определения состояния рюкзака
void backpack_recalc(Backpack* b) {
    //обнуляем общий вес рюкзака чтобы при вес не прибавлялся к результату предыдущего расчёта
    b->total_weight = 0;
    for (int i = 0; i < BACKPACK_SLOTS; i++) {
        //вес предмета прибавляем к общему весу рюкзака
        b->total_weight += item_weight(b->slots[i]);
    }

    if (b->total_weight * PERCENT_FULL <= b->max_capacity * LIGHT_PERCENT) {
        b->state = LOAD_LIGHT;
    }

    else if (b->total_weight <= b->max_capacity) {
        b->state = LOAD_NORMAL;
    }

    else {
        b->state = LOAD_OVERLOAD;
    }
}


// начальная настройка рюкзака
void backpack_init(Backpack* b) {
    for (int i = 0; i < BACKPACK_SLOTS; i++) {
        b->slots[i] = Empty;
    }
    b->max_capacity = BASE_CAPACITY;   // начальная максимальную вместимость рюкзака
    backpack_recalc(b);
}


//добавление предмета в указанную ячейку рюкзака
// b — указатель на рюкзак
int backpack_put(Backpack* b, int slot, int item_id) {

    if (slot < 0 || slot >= BACKPACK_SLOTS ||
        item_id < 0 || item_id >= ItemCount) {
        return 0;
    }
    // записываем идентификатор предмета в указ. ячейку
    b->slots[slot] = item_id;
    // и обновляем состояние загрузки рюкзака
    backpack_recalc(b);
    return 1;
}


//удаление предмета [5]
void backpack_remove(Backpack* b, int slot) {
    if (slot < 0 || slot >= BACKPACK_SLOTS) {
        return;
    }
    b->slots[slot] = Empty;
    backpack_recalc(b);
}


//удаления повторяющихся предметов [6]
void backpack_remove_duplicates(Backpack* b) {
    for (int i = 0; i < BACKPACK_SLOTS; i++) {
        for (int j = i + 1; j < BACKPACK_SLOTS; j++) {
            if (b->slots[j] == b->slots[i] && b->slots[i] != Empty) {
                b->slots[j] = Empty;
            }
        }
    }
    backpack_recalc(b);
}


// увеличение максимальной вместимости рюкзака
void backpack_add_capacity(Backpack* b, int extra) {
    b->max_capacity += extra;
    backpack_recalc(b);
}


//вывода содержимого рюкзака и его характеристик
void backpack_print(const Backpack* b, const * c) {
    for (int i = 0; i < BACKPACK_SLOTS; i++) {

        printf("  Слот %d: [%d] %s (вес %d)\n",
            i,                              // номер ячейки
            b->slots[i],                    // id предмета
            item_name(c, b->slots[i]),      // Название
            item_weight(b->slots[i]));      // вес
    }

    printf("  Вес: %d/%d, состояние: %s\n",
        b->total_weight,             // общий вес
        b->max_capacity,             // вес макс.
        load_state_name(b->state));  // состояние рюкзака
}