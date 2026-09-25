#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
//подключаем инструменты для работы с вводом и выводом далее MAIN начало программы
#define INV_SIZE 10
int main()
{
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	int current_day = 1;
	int current_hour = 8;

	int running = 1;
	// создадим целочисл. переменную (для выбора игрока в меню)
	int choice;

	int inventory[INV_SIZE] = { 0 };
	//создаем массив из 10 эл. и chair хранит символы в адрес.строке
	// * указывает на строку символов
	char* item_names[10] = {
		"Пусто",
		"Дерево",
		"Камень",
		"Семена",
		"Морковка",
		"Ведро воды",
		"Земля",
		"Рыба",
		"Удочка",
		"Лопата"
	};

	inventory[0] = 1;
	inventory[1] = 2;
	inventory[2] = 3;
	inventory[3] = 4;
	inventory[4] = 5;
	inventory[5] = 6;
	inventory[6] = 7;
	inventory[7] = 8;
	inventory[8] = 9;
	inventory[9] = 0;

	while (running)
	{
		printf("\n===== МЕНЮ ФЕРМЕРА ====\n"
			"[0] Выход\n"
			"[1] Посмотреть на часы\n"
			"[2] Перемотать время(поработать)\n"
			"[3] Посмотреть инвентарь\n"
			"[4] Положить предмет в слот\n"
			"[5] Выбросить предмет\n"
			"[6] Выполнить задание по варианту:\n"
			"Выберите действие: \n");

		scanf("%d", &choice);

		switch (choice)
		{
		case 0:
			running = 0;
			break;

		case 1:
			printf("Текущее время: День %d, %02d:00\n", current_day, current_hour);
			break;

		case 2:
		{
			int hours;
			printf("Сколько часов вы хотите потратить на работу? ");
			scanf("%d", &hours);
			current_hour += hours;
			while (current_hour >= 24)
			{
				current_hour -= 24;
				current_day++;
			}
			printf("Новое время: День %d, %02d:00\n", current_day, current_hour);
		}
		break;

		case 3:
			for (int i = 0; i < INV_SIZE; i++)
			{
				printf("Слот %d: [%d] ", i, inventory[i]);
				printf("(%s)\n", item_names[inventory[i]]);
			}
			break;

		case 4: {
			int slot;
			int item_id;
			printf("Введите номер слота(0-9): \n");
			scanf("%d", &slot);
			if (slot < 0 || slot >= INV_SIZE)
			{
				printf("Такого слота нет (-_-), меняй \n");
				break;
			}
			printf("Введите ID предмета: \n");
			scanf("%d", &item_id);
			if (item_id < 0 || item_id >= INV_SIZE)
			{
				printf("Такого ID нет (-_-),меняй \n");
				break;
			}
			inventory[slot] = item_id;
			printf("Предмет %d помещен в слот %d\n", slot);
			break;
		}

		}
	}
	return 0;
}