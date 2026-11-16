#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>  
#include <string.h>
#include "utils.h"

// функция для очистки буфера ввода
void clear_input(void) {
    int c;
    // считываем символы по одному пока не встретим символ новой строки или конец файла
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}
// функция для ввода целого числа с проверкой допустимого диапазона
int read_int(const char* prompt, int min, int max) {
    int value;
    while (1) {
        printf("%s", prompt);
        int r = scanf("%d", &value);
        // если дошли до концв файла возвращаем минимальное допустимое значение
        if (r == EOF) {
            return min;
        }
        clear_input();
        if (r != 1) {
            printf("Причём тут буквы..? Число введи.\n");
        }
        else if (value < min || value > max) {
            printf("Число должно быть от %d до %d.\n", min, max);
        }
        // Если все проверки пройдены, возвращаем введённое число.
        else {
            return value;
        }
    }
}

// функция для ввода непустой строки
// buf — массив в который записываеться введенная строка
void read_line(const char* prompt, char* buf, int size) {

    while (1) {
        printf("%s", prompt);
        if (fgets(buf, size, stdin) == NULL) {
            // если считать строку не удалось записываем в начало массива
            buf[0] = '\0';
            return;
        }
        //если \n в строке нет значит ввод длиннее 31 - выбрасываем остаток
        if (strchr(buf, '\n') == NULL) {
            clear_input();
        }
        // \n удаляем --> записываем \0
        buf[strcspn(buf, "\n")] = '\0';

        // проверяем, что после удаления символа новой строки строка не оказалась пустой.
        if (buf[0] != '\0') {
            return;
        }
        //если пользователь ниче не ввел
        printf("Строка не должна быть пустой.\n");
    }
}

//вывод текущего времени
void show_time(const GameTime* t) {
    printf("Текущее время: День %d, %02d:00\n", t->day, t->hour);
}

//сколько часов можно ждать до 02:00 (0 - уже ночь)
int hours_until_night(const GameTime* t) {
    if (t->hour >= NIGHT_HOUR && t->hour < MORNING_HOUR) {
        return 0;
    }
    if (t->hour >= MORNING_HOUR) {
        return HOURS_IN_DAY - t->hour + NIGHT_HOUR;
    }
    return NIGHT_HOUR - t->hour;
}

//сдвиг времени вперёд на hours часов
void advance_time(GameTime* t, int hours) {
    t->hour += hours;
    while (t->hour >= HOURS_IN_DAY) {
        t->hour -= HOURS_IN_DAY;
        t->day++;
    }
}

//ожидание, но не дальше ближайших 02:00
void wait_time(GameTime* t) {
    int limit = hours_until_night(t);
    if (limit == 0) {
        printf("Уже ночь, пора спать!\n");
        return;
    }
    printf("Можно ждать от 1 до %d ч.\n", limit);
    int h = read_int("Сколько часов ждать? ", 1, limit);
    advance_time(t, h);
    show_time(t);
}