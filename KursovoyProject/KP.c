#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>


#define MAX_RECORDS 100           // ������������ ���������� ���������� ������� � ���� ������
#define FILE_NAME "PCcase.txt"    // �������� �����

// ��������� ��� �������� ���������� � ������� ��
typedef struct {
    char developer[50];           // ������������� ������� ��
    char form_factor[30];         // ����-������ ����������� ����� � �������
    char size[30];                // ������ ������� �� (Full-Tower, Mid-Tower, Mini-Tower, Super-Tower)
    int height;                   // ������ ������� ��
    char color[20];               // ���� ������� ��
} pc_case_t;

pc_case_t records[MAX_RECORDS];  // ������� ������ �������, ������� ����� ������� ��� ������� � �� ����������������
int record_count = 0;            // ���������� ��� �������� ���������� ������� � ������� records

// ����� ���������� �������
int add_record();
int save_to_file();
int load_from_file();
int search_record();
int compare_by_color(const void* a, const void* b);
int sort_records();
int print_all_records();
int menu();

// �������� ���������
int main() {
    system("chcp 1251");
    menu();           // ����� ���� ���������
    return 0;
}

// ������� ���������� ����� ������
int add_record() {
    if (record_count >= MAX_RECORDS) {             // �������� �� �������� �� ����� ������� MAX_RECORDS
        printf("���� ������ �����������!\n");      // ���� ����� ��������, �� ��������� �����������
        return;
    }
    // ������ ������ � ���������� �� � ������ records
    printf("������� �������������: ");
    scanf("%s", records[record_count].developer);
    printf("������� ����-������ �����: ");
    scanf("%s", records[record_count].form_factor);
    printf("������� ���������� �������: ");
    scanf("%s", records[record_count].size);
    int valid = 0;
    while (!valid) {
        printf("������� ������ �������: ");
        if (scanf("%d", &records[record_count].height) == 1) {
            valid = 1;                                            // ���� ������������ ���� ���������� ����� �� ������������ 1
        }
        else {
            // ���� ������������ ���� ����� ��� ������ ������� �� ������������ 0 � ��������� ��������� �� ������
            printf("������: ������ ������ ���� ������!\n");
            // ������� �����, ����� �� ���� ������������ � ��������� ���������� ����������� ����, ���� ������������ �� ������ ���������� �����
            while (getchar() != '\n');
        }
    }
    printf("������� ���� �������: ");
    scanf("%s", records[record_count].color);

    record_count++;                               // ����� ����� ����������� ������
    printf("������ ��������� �������!\n");

    // �������������� ����� ���������� ������
    if (save_to_file() != 0) {
        printf("������ ��� ���������� � ����!\n");
        return 1;  // ������ ����������
    }

    return 0;
}

// ������� ���������� ������� � ����
int save_to_file() {
    FILE* file = fopen(FILE_NAME, "w");           // ��������� ���� � ������ ������
    if (!file) {                                  // ���� ���� �� ������� �������, �� ��������� ��������� �� ������
        printf("������ ��� �������� �����!\n");
        return 1;
    }

    for (int i = 0; i < record_count; i++) {      // ������ ������������ � ����
        fprintf(file, "%s %s %s %d %s\n",
            records[i].developer,
            records[i].form_factor,
            records[i].size,
            records[i].height,
            records[i].color);
    }
    fclose(file);                                 // ���� �����������
    printf("������ ��������� � ����.\n");
    return 0;
}

// ������� �������� ������� �� �����
int load_from_file() {
    FILE* file = fopen(FILE_NAME, "r");          // ��������� ���� � ������ ������
    if (!file) {                                 // ���� ���� �� ������� �������, �� ��������� ��������� �� ������
        printf("���� �� ������.\n");
        return 1;
    }

    record_count = 0;
    while (fscanf(file, "%s %s %s %d %s",
        records[record_count].developer,
        records[record_count].form_factor,
        records[record_count].size,
        &records[record_count].height,
        records[record_count].color) == 5) {     // ������� fscanf() ������ ������� ������� ����� 5 �������� �� ����� ��� 1 ������
        record_count++;
    }
    fclose(file);                                // ���� �����������
    printf("������ ��������� �� �����.\n");
    return 0;
}

// ������� ������ ������
int search_record() {
    int choice;                                  // ����� ����������
    char developer_key[50];                      // ������ ��� ���������� �� �������������
    char size_key[30];                           // ������ ��� ���������� �� ������� �������
    int found = 0;

    printf("�������� �������� ������:\n");
    printf("1 - �� �������������\n");
    printf("2 - �� �������\n");
    printf("3 - �� ������������� � �������\n");
    printf("��� �����: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:  // ����� �� �������������
        printf("������� ������������� ��� ������: ");
        scanf("%s", developer_key);
        for (int i = 0; i < record_count; i++) {
            if (strcmp(records[i].developer, developer_key) == 0) {      // ������������ ������ ������������� � ������ � ������������� ������������
                printf("�������: %s %s %s %d %s\n",
                    records[i].developer,
                    records[i].form_factor,
                    records[i].size,
                    records[i].height,
                    records[i].color);
                found = 1;
            }
        }
        break;

    case 2:  // ����� �� �������
        printf("������� ���������� ��� ������: ");
        scanf("%s", size_key);
        for (int i = 0; i < record_count; i++) {
            if (strcmp(records[i].size, size_key) == 0) {             // ������������ ������� ������� � ������ � ������ ������� ������������
                printf("�������: %s %s %s %d %s\n",
                    records[i].developer,
                    records[i].form_factor,
                    records[i].size,
                    records[i].height,
                    records[i].color);
                found = 1;
            }
        }
        break;

    case 3:  // ����� �� ������������� � �������
        printf("������� ������������� ��� ������: ");
        scanf("%s", developer_key);
        printf("������� ���������� ��� ������: ");
        scanf("%s", size_key);
        for (int i = 0; i < record_count; i++) {
            if (strcmp(records[i].developer, developer_key) == 0 &&  /* ������������ ������� ������� c �������������� � ������ � ������ ������� � ������������� ������������
                                                                                                   */
                strcmp(records[i].size, size_key) == 0) {
                printf("�������: %s %s %s %d %s\n",
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
        printf("������������ �����. ���������� �����.\n");
        return 1;  // ������ �����
    }

    if (!found) {
        printf("������ �� �������.\n");
        return 1;  // ������ �� �������
    }
    return 0;  // �����
}


int sort_records() {
    if (record_count == 0) {
        printf("��� ������� ��� ����������.\n");
        return 1;
    }
    qsort(records, record_count, sizeof(pc_case_t), compare_by_color);
    printf("������ ������������� �� �����.\n");
    return 0;
}

// ������� ������ ���� �������
int print_all_records() {
    if (record_count == 0) {
        printf("���� ������ �����.\n");
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

// ������� ���������� �� �����
int compare_by_color(const void* a, const void* b) {
    return strcmp(((pc_case_t*)a)->color, ((pc_case_t*)b)->color);
}

// ���� ���������
int menu() {
    int choice;
    while (1) {
        printf("\n����:\n");
        printf("1 - �������� ������\n");
        printf("2 - ����� ������\n");
        printf("3 - ��������� � ����\n");
        printf("4 - ��������� �� �����\n");
        printf("5 - �������� ��� ������\n");
        printf("6 - ����������� �� �����\n");
        printf("7 - �����\n");
        printf("�������� ��������: ");
        scanf("%d", &choice);

        // ���������� �������� ��������� �������������
        switch (choice) {
        case 1:
            add_record();  // ���������� ������
            break;
        case 2:
            search_record(); // ����� ������
            break;
        case 3:
            save_to_file();  // ���������� � ����
            break;
        case 4:
            load_from_file(); // �������� �� �����
            break;
        case 5:
            print_all_records(); // ����������� ���� �������
            break;
        case 6:
            sort_records();  // ���������� ������� �� ����� �������
            break;
        case 7:
            printf("�����...\n"); // ����� �� ���������
            return 0;
        default:
            printf("������������ ����. ������� ��� ���.\n"); // ������ � ������ ������ ����� ��� ��������� �� 1 �� 7
        }
    }
    return 0;
}