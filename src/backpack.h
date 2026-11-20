#ifndef BACKPACK_H
#define BACKPACK_H
#define BACKPACK_SLOTS   10
#define BASE_CAPACITY    30 //начальная вместимость
#define POUCH_CAPACITY   10 // доп вместимость(подсумка)
#define POUCH_COST       50 // стоимость подсумка
#define LIGHT_PERCENT    30 // процент вместимости "налегке"
#define PERCENT_FULL     100
#define ITEM_NAME_LEN    32           // максимальная длина названия предмета
#define ITEMS_FILE       "items.txt"  // файл с названиями предметов

// веса предметов
#define WEIGHT_EMPTY         0
#define WEIGHT_WOOD          3
#define WEIGHT_STONE         5
#define WEIGHT_SEEDS         1
#define WEIGHT_CARROT        1
#define WEIGHT_WATER_BUCKET  4
#define WEIGHT_SOIL          4
#define WEIGHT_FISH          2
#define WEIGHT_FISHING_ROD   3
#define WEIGHT_SHOVEL        4

// typedef enum позволяет создать собственный тип данных (ItemId)
typedef enum {
    Empty = 0,
    Wood,
    Stone,
    Seeds,
    Carrot,
    WaterBucket,
    Soil,
    Fish,
    FishingRod,
    Shovel,
    ItemCount          // количество предметов, не сам предмет
} ItemId;


// состояния загрузки рюкзака
typedef enum {
    LOAD_LIGHT,
    LOAD_NORMAL,
    LOAD_OVERLOAD
} LoadState;


// структура для хранения информации о рюкзаке игрока
// typedef struct что бы обращятся к Backpack
typedef struct {
    int slots[BACKPACK_SLOTS];
    int total_weight;               // общий вес всех предметов
    int max_capacity;               // максимальный вес
    LoadState state;                // тек. состояние загрузки
} Backpack;


// каталог названий предметов (читается из items.txt)
typedef struct {
    char names[ItemCount][ITEM_NAME_LEN];
} ItemCatalog;


void catalog_init(ItemCatalog* c);
const char* item_name(const ItemCatalog* c, int item_id);
void backpack_init(Backpack* b);
void backpack_recalc(Backpack* b);
int backpack_put(Backpack* b, int slot, int item_id);
void backpack_remove(Backpack* b, int slot);
void backpack_remove_duplicates(Backpack* b);
void backpack_add_capacity(Backpack* b, int extra);
void backpack_print(const Backpack* b, const ItemCatalog* c);
int item_weight(int item_id);
const char* load_state_name(LoadState s);

#endif