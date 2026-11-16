#ifndef UTILS_H
#define UTILS_H
//константы времени (чтобы не было магических чисел)
#define NIGHT_HOUR    2
#define MORNING_HOUR  8
#define HOURS_IN_DAY  24
#define START_DAY     1

//структура игрового времени
typedef struct {
    int day;
    int hour;
} GameTime;

//функция для очистки входного буфера
void clear_input(void);

//функция для ввода целого числа с проверкой диапазона
int read_int(const char* prompt, int min, int max);

// объявление функции для ввода строки
void read_line(const char* prompt, char* buf, int size);

//функции для работы со временем
void show_time(const GameTime* t);
int  hours_until_night(const GameTime* t);
void advance_time(GameTime* t, int hours);
void wait_time(GameTime* t);

#endif 
// UTILS_H — конец защиты от повторного подключения заголовочного файладключения заголовочного файла