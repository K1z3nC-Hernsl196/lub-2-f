#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <string.h>
//подключаем инструменты для работы с вводом,выводом и прочее
#define INV_SIZE 10
#define MAX_ITEMS 10
#define MAX_NAME_LEN 32 //+ 1 под конец строки в С (\0)
#define MAX_FARMER_NAME 33
//char тип "символ"
char farmer_name[MAX_FARMER_NAME] = "Фермер";

int load_items(char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
	//работа с открытым файлом
	FILE* file = fopen("items.txt", "r");
	if (file == NULL) {
		perror("Ошибка открытия");
		return 0;
	}
	for (int i = 0; i < MAX_ITEMS; i++) {
		if (fgets(item_names[i], MAX_NAME_LEN, file) == NULL) {

			fclose(file);
			return 0;
		}
		//убираем символы переноса строки т.к fgets может сохранить их в названии
		item_names[i][strcspn(item_names[i], "\n")] = '\0';
	}
		fclose(file);
		return 1;
}


void show_menu(void) {
	printf("\n===== МЕНЮ ФЕРМЕРА %s ====\n"
		"[0] Выход\n"
		"[1] Посмотреть на часы\n"
		"[2] Перемотать время(поработать)\n"
		"[3] Посмотреть инвентарь\n"
		"[4] Положить предмет в слот\n"
		"[5] Выбросить предмет\n"
		"[6] Удалить повторы из инвентаря:\n"
		"Выберите действие: ", farmer_name);
}

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
	char item_names[MAX_ITEMS][MAX_NAME_LEN];
	if (!load_items(item_names)) {
		printf("Не удалось загрузить item.txt\n");
		return 1;
	}

	printf("Введите имя фермера: ");
	if (scanf("%32s", farmer_name) != 1) { //читаем строку до 32 симв без пробелов
		farmer_name[strcspn(farmer_name, "\n")] = '\0'; // если ниче не ввели
		strcpy(farmer_name, "Фермер"); // остаеться по дефолту фермер
	}
	while (getchar() != '\n'); //очищаем буфер от остатков 

	while (running)
	{
		show_menu();

		//если польз. ввел буквы (т.е. scanf = 0) просим ввести число
		if (scanf("%d", &choice) != 1) {
			printf("Какие буквы? Только числа.");
		//getchar считываепт по одному символу до Enter
			while (getchar() != '\n');
		//пропускает остаток текущей итерации и начинате заново
			continue;
		}
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
			if (scanf("%d", &hours) != 1){
				printf("Какие буквы? Только числа.");
				while (getchar() != '\n');
				break;
			}
			
			if (hours < 0) {
				printf("Количество часов неможет быть отрицательным!");
				break;
			}
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
				printf("Слот %d: [%d] %s\n", i, inventory[i], item_names[inventory[i]]);
			}
			break;

		case 4: 
		{
			int slot;
			int item_id;
			printf("Введите номер слота(0-9): \n");
			if (scanf("%d", &slot) != 1) {
				printf("Какие буквы? Только числа.");
				while (getchar() != '\n');
				break;
			}
			if (slot < 0 || slot >= INV_SIZE)
			{
				printf("Такого слота нет\n");
				break;
			}
			printf("Введите ID предмета: \n");
			if (scanf("%d", &item_id) != 1) {

				printf("Какие буквы? Только числа.");
				while (getchar() != '\n');
				break;
			}
			if (item_id < 0 || item_id >= INV_SIZE)
			{
				printf("Такого ID нет\n");
				break;
			}
			inventory[slot] = item_id;
			printf("Предмет помещен в слот %d\n", slot);
			break;
		}
		case 5: {
			int slot;

			printf("Введите номер слота(0-9): \n");
			if (scanf("%d", &slot) != 1) {
				printf("Какие буквы? Только числа.");
				while (getchar() != '\n');
				break;
			}
			if (slot < 0 || slot >= INV_SIZE)
			{
				printf("Такого слота нет\n");
				break;
			}
			inventory[slot] = 0;
			printf("Предмет вброшен из слота %d\n", slot);
			break;
		}
		case 6: 

			for (int i = 0; i < INV_SIZE; i++)
			{
				printf("Слот %d: [%d] %s\n", i, inventory[i], item_names[inventory[i]]);
			}
		
				for (int i = 0; i < INV_SIZE; i++)
				for (int j = i + 1; j < INV_SIZE; j++)
				{
					if (inventory[j] == inventory[i] && inventory[i] != 0)
					{
						inventory[j] = 0;
					}
				}

			printf("Измененный инвентарь:\n");

			for (int i = 0; i < INV_SIZE; i++) 
			{
				printf("Слот %d: [%d] %s\n", i, inventory[i], item_names[inventory[i]]);

			}
			break;
		default:
			printf("Такого пункта в меню нет.");
			break;
		}
	}
	return 0;
}