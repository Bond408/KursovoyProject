#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>


#define MAX_RECORDS 100           // Максимальное количество допустимых записей в базе данных
#define FILE_NAME "PCcase.txt"    // Название файла

// Структура для хранения информации о корпусе ПК
typedef struct {
    char developer[50];           // Производитель корпуса ПК
    char form_factor[30];         // Форм-фактор материнской платы в корпусе
    char size[30];                // Размер корпуса ПК (Full-Tower, Mid-Tower, Mini-Tower, Super-Tower)
    int height;                   // Высота корпуса ПК
    char color[20];               // Цвет корпуса ПК
} pc_case_t;

pc_case_t records[MAX_RECORDS];  // Создаем массив записей, который будет хранить все корпуса с их характеристиками
int record_count = 0;            // Переменная для хранения количества записей в массиве records

// Вызов прототипов функций
int add_record();
int save_to_file();
int load_from_file();
int search_record();
int compare_by_color(const void* a, const void* b);
int sort_records();
int print_all_records();
int menu();

// Основная программа
int main() {
    system("chcp 1251");
    menu();           // Вызов меню программы
    return 0;
}

// Функция добавления новой записи
int add_record() {
    if (record_count >= MAX_RECORDS) {             // Проверка не превышен ли лимит записей MAX_RECORDS
        printf("База данных переполнена!\n");      // Если лимит превышен, то программа прерывается
        return;
    }
    // Запрос данных и сохранение их в массив records
    printf("Введите производителя: ");
    scanf("%s", records[record_count].developer);
    printf("Введите форм-фактор платы: ");
    scanf("%s", records[record_count].form_factor);
    printf("Введите типоразмер корпуса: ");
    scanf("%s", records[record_count].size);
    int valid = 0;
    while (!valid) {
        printf("Введите высоту корпуса: ");
        if (scanf("%d", &records[record_count].height) == 1) {
            valid = 1;                                            // Если пользователь ввел корректное число то возвращается 1
        }
        else {
            // Если пользователь ввел буквы или другие символы то возвращается 0 и выводится сообщение об ошибке
            printf("Ошибка: высота должна быть числом!\n");
            // Очистка ввода, чтобы не было зацикливания и программа продолжает запрашивать ввод, пока пользователь не введет корректное число
            while (getchar() != '\n');
        }
    }
    printf("Введите цвет корпуса: ");
    scanf("%s", records[record_count].color);

    record_count++;                               // После ввода добавляется запись
    printf("Запись добавлена успешно!\n");

    // Автосохранение после добавления записи
    if (save_to_file() != 0) {
        printf("Ошибка при сохранении в файл!\n");
        return 1;  // Ошибка сохранения
    }

    return 0;
}

// Функция сохранения записей в файл
int save_to_file() {
    FILE* file = fopen(FILE_NAME, "w");           // Открываем файл в режиме записи
    if (!file) {                                  // Если файл не удается открыть, то выводится сообщение об ошибке
        printf("Ошибка при открытии файла!\n");
        return 1;
    }

    for (int i = 0; i < record_count; i++) {      // Записи записываются в файл
        fprintf(file, "%s %s %s %d %s\n",
            records[i].developer,
            records[i].form_factor,
            records[i].size,
            records[i].height,
            records[i].color);
    }
    fclose(file);                                 // Файл закрывается
    printf("Данные сохранены в файл.\n");
    return 0;
}

// Функция загрузки записей из файла
int load_from_file() {
    FILE* file = fopen(FILE_NAME, "r");          // Открываем файл в режиме чтения
    if (!file) {                                 // Если файл не удается открыть, то выводится сообщение об ошибке
        printf("Файл не найден.\n");
        return 1;
    }

    record_count = 0;
    while (fscanf(file, "%s %s %s %d %s",
        records[record_count].developer,
        records[record_count].form_factor,
        records[record_count].size,
        &records[record_count].height,
        records[record_count].color) == 5) {     // Функция fscanf() должна усмешно считать равно 5 значений из файла для 1 записи
        record_count++;
    }
    fclose(file);                                // Файл закрывается
    printf("Данные загружены из файла.\n");
    return 0;
}

// Функция поиска записи
int search_record() {
    int choice;                                  // Выбор сортировки
    char developer_key[50];                      // Массив для сортировки по производителю
    char size_key[30];                           // Массив для сортировки по размеру корпуса
    int found = 0;

    printf("Выберите критерий поиска:\n");
    printf("1 - По производителю\n");
    printf("2 - По размеру\n");
    printf("3 - По производителю и размеру\n");
    printf("Ваш выбор: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:  // Поиск по производителю
        printf("Введите производителя для поиска: ");
        scanf("%s", developer_key);
        for (int i = 0; i < record_count; i++) {
            if (strcmp(records[i].developer, developer_key) == 0) {      // Сравниваются строки производителя в записи и производитель пользователя
                printf("Найдено: %s %s %s %d %s\n",
                    records[i].developer,
                    records[i].form_factor,
                    records[i].size,
                    records[i].height,
                    records[i].color);
                found = 1;
            }
        }
        break;

    case 2:  // Поиск по размеру
        printf("Введите типоразмер для поиска: ");
        scanf("%s", size_key);
        for (int i = 0; i < record_count; i++) {
            if (strcmp(records[i].size, size_key) == 0) {             // Сравниваются размеры корпуса в записи и размер корпуса пользователя
                printf("Найдено: %s %s %s %d %s\n",
                    records[i].developer,
                    records[i].form_factor,
                    records[i].size,
                    records[i].height,
                    records[i].color);
                found = 1;
            }
        }
        break;

    case 3:  // Поиск по производителю и размеру
        printf("Введите производителя для поиска: ");
        scanf("%s", developer_key);
        printf("Введите типоразмер для поиска: ");
        scanf("%s", size_key);
        for (int i = 0; i < record_count; i++) {
            if (strcmp(records[i].developer, developer_key) == 0 &&  /* Сравниваются размеры корпуса c производителем в записи и размер корпуса и производитель пользователя
                                                                                                   */
                strcmp(records[i].size, size_key) == 0) {
                printf("Найдено: %s %s %s %d %s\n",
                    records[i].developer,
                    records[i].form_factor,
                    records[i].size,
                    records[i].height,
                    records[i].color);
                found = 1;
            }
        }
        break;

    default:
        printf("Некорректный выбор. Попробуйте снова.\n");
        return 1;  // Ошибка ввода
    }

    if (!found) {
        printf("Записи не найдены.\n");
        return 1;  // Ничего не найдено
    }
    return 0;  // Успех
}


int sort_records() {
    if (record_count == 0) {
        printf("Нет записей для сортировки.\n");
        return 1;
    }
    qsort(records, record_count, sizeof(pc_case_t), compare_by_color);
    printf("Записи отсортированы по цвету.\n");
    return 0;
}

// Функция вывода всех записей
int print_all_records() {
    if (record_count == 0) {
        printf("База данных пуста.\n");
        return 1;
    }

    for (int i = 0; i < record_count; i++) {
        printf("%s %s %s %d %s\n",
            records[i].developer,
            records[i].form_factor,
            records[i].size,
            records[i].height,
            records[i].color);
    }
    return 0;
}

// Функция сортировки по цвету
int compare_by_color(const void* a, const void* b) {
    return strcmp(((pc_case_t*)a)->color, ((pc_case_t*)b)->color);
}

// Меню программы
int menu() {
    int choice;
    while (1) {
        printf("\nМеню:\n");
        printf("1 - Добавить запись\n");
        printf("2 - Поиск записи\n");
        printf("3 - Сохранить в файл\n");
        printf("4 - Загрузить из файла\n");
        printf("5 - Показать все записи\n");
        printf("6 - Сортировать по цвету\n");
        printf("7 - Выйти\n");
        printf("Выберите действие: ");
        scanf("%d", &choice);

        // Выполнение операции выбранной пользователем
        switch (choice) {
        case 1:
            add_record();  // Добавление записи
            break;
        case 2:
            search_record(); // Поиск записи
            break;
        case 3:
            save_to_file();  // Сохранение в файл
            break;
        case 4:
            load_from_file(); // Загрузка из файла
            break;
        case 5:
            print_all_records(); // Отображение всех записей
            break;
        case 6:
            sort_records();  // Сортировка записей по цвету корпуса
            break;
        case 7:
            printf("Выход...\n"); // Выход из программы
            return 0;
        default:
            printf("Некорректный ввод. Давайте еще раз.\n"); // Строка в случае выбора цифры вне диапазона от 1 до 7
        }
    }
    return 0;
}