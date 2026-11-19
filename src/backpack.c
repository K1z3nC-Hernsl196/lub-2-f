#include <stdio.h>
#include "backpack.h"

//определения веса предмета по его идентификатору
int item_weight(int item_id) {
    // статический массив, в котором хранятся веса всех предметов
    static const int weights[ItemCount] = { 0, 3, 5, 1, 1, 4, 4, 2, 3, 4 };
    // проверяем, входит ли идентификатор предмета в допустимый диапазон
    if (item_id < 0 || item_id >= ItemCount) {
        return 0;
    }
    // если все нром возвращаем вес предмета из масива по индек.
    return weights[item_id];
}


// функция для получения названия предмета по его идентификатору
// (возвращает указатель на строку с названием предмета)
const char* item_name(int item_id) {
    //статический массив указателей на строки
    static const char* const names[ItemCount] = {
        "Empty", "Wood", "Stone", "Seeds", "Carrot",
        "Water Bucket", "Soil", "Fish", "Fishing Rod", "Shovel"
    };
    if (item_id < 0 || item_id >= ItemCount) {
        return "???";
    }
    return names[item_id];
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
void backpack_print(const Backpack* b) {
    for (int i = 0; i < BACKPACK_SLOTS; i++) {

        printf("  Слот %d: [%d] %s (вес %d)\n",
            i,                          // номер ячейки
            b->slots[i],                // id предмета
            item_name(b->slots[i]),     // Название
            item_weight(b->slots[i]));  // вес
    }

    printf("  Вес: %d/%d, состояние: %s\n",
        b->total_weight,             // общий вес 
        b->max_capacity,             // вес макс.
        load_state_name(b->state));  // состояние рюкзака
}