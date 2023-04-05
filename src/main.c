#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <windows.h>
#include "functions.h"


int main(int argc, char* argv[]) {
	SetConsoleOutputCP(CP_UTF8); // установить кодировку консоли в UTF-8
    sqlite3 *db;
    char *errMsg = 0;
    int rc;
    
    rc = sqlite3_open("buildings.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    
    // создаем таблицу district
/*    char *sql_create = "CREATE TABLE district (id INTEGER PRIMARY KEY, dist_name TEXT, dist_description TEXT)";
    rc = sqlite3_exec(db, sql_create, NULL, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        exit(1);
    }
*/
	// Включаем режим autocommit
    sqlite3_exec(db, "PRAGMA autocommit = ON", NULL, NULL, &errMsg);

	// Начинаем транзакцию
    rc = sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        exit(1);
    }
    
    // Вставляем данные в таблицу district
    char *sql_insert1 = "INSERT INTO districts (dist_name, dist_description) VALUES ('Московский', 'Хороший район')";
    rc = sqlite3_exec(db, sql_insert1, NULL, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        exit(1);
    }
    
    // Вставляем еще одну строку в таблицу district
    char *sql_insert2 = "INSERT INTO districts (dist_name, dist_description) VALUES ('Ленинский', 'Средний район')";
    rc = sqlite3_exec(db, sql_insert2, NULL, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        exit(1);
    }
    
    // Завершаем транзакцию
    rc = sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        exit(1);
    }
    
    // Отключаем режим autocommit
    sqlite3_exec(db, "PRAGMA autocommit = OFF", NULL, NULL, &errMsg);
	
/*    // добавляем новые столбцы в таблицу buildings
    char *sql_check = "SELECT district_id FROM buildings LIMIT 1";
    rc = sqlite3_exec(db, sql_check, NULL, 0, &errMsg);
    if (rc = SQLITE_OK) {
    	// столбец уже есть
	printf("Столбец district_id уже существует\n");
    } else {
    char *sql_alter = "ALTER TABLE buildings ADD COLUMN district_id INTEGER";
    rc = sqlite3_exec(db, sql_alter, NULL, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        exit(1);
    } 
}*/
	
    int choice = 0;
    do {
        printf("Выберите действие:\n");
        printf("1. Здания, у которых срок эксплуатации больше > 50 лет.\n");
		printf("2. Вывести все здания.\n");
		printf("3. Подсчет количества зданий со сроком эксплуатации 14 лет.\n");
		printf("4. Суммируем количество квартир в зданиях со сроком эксплуатации более 49 лет.\n");
		printf("5. Вывести максимальный и минимальный срок эксплуатации зданий.\n");
		printf("6. Вывести полные сведения о зданиях и районах расположения для района Московский.\n");
        printf("7. Выход.\n");
        printf("Ваш выбор: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                {
                    char *sql = "SELECT * FROM buildings WHERE lifespan > 50";
                    rc = sqlite3_exec(db, sql, callback, 0, &errMsg);
                    if (rc != SQLITE_OK) {
                        fprintf(stderr, "SQL error: %s\n", errMsg);
                        sqlite3_free(errMsg);
                    }
                    break;
                }
			case 2:
				{
					// выводим данные обо всех зданиях
					char *sql_select = "SELECT buildings.id, address, lifespan, time_to_major_repair, district_id FROM buildings INNER JOIN districts ON district_id= districts.id";
					rc = sqlite3_exec(db, sql_select, callback, 0, &errMsg);
					if (rc != SQLITE_OK) {
						fprintf(stderr, "SQL error: %s\n", errMsg);
						sqlite3_free(errMsg);
					}
					break;
				}
            case 3:
                {
					// подсчитываем количество зданий со сроком эксплуатации 14 лет
					char *sql_count = "SELECT COUNT(*) FROM buildings WHERE lifespan=14";
					rc = sqlite3_exec(db, sql_count, callback, 0, &errMsg);
					if (rc != SQLITE_OK) {
						fprintf(stderr, "SQL error: %s\n", errMsg);
						sqlite3_free(errMsg);
					}
					break;
                }
			case 4:
				{
					// суммируем количество квартир в зданиях со сроком эксплуатации более 49 лет
					char *sql_sum = "SELECT SUM(num_apartments) FROM buildings WHERE lifespan>49";
					rc = sqlite3_exec(db, sql_sum, callback, 0, &errMsg);
					if (rc != SQLITE_OK) {
						fprintf(stderr, "SQL error: %s\n", errMsg);
						sqlite3_free(errMsg);
					}
					break;
				}
			case 5:
				{
					// Выводим максимальный и минимальный срок эксплуатации зданий
					printf("Максимальный срок эксплуатации: ");
					char *sql = "SELECT MAX(lifespan) FROM buildings";
					rc = sqlite3_exec(db, sql, callback, 0, &errMsg);
					if (rc != SQLITE_OK) {
						fprintf(stderr, "SQL error: %s\n", errMsg);
						sqlite3_free(errMsg);
					}
					printf("Минимальный срок эксплуатации: ");
					sql = "SELECT MIN(lifespan) FROM buildings";
					rc = sqlite3_exec(db, sql, callback, 0, &errMsg);
					if (rc != SQLITE_OK) {
						fprintf(stderr, "SQL error: %s\n", errMsg);
						sqlite3_free(errMsg);
					}
					break;
				}
			case 6:
				{
					// Выводим полные сведения о зданиях и районах расположения для района = "Московский"
					printf("Выводим полные сведения о зданиях и районах расположения для района Московский:\n");
					char *sql = "SELECT * FROM buildings INNER JOIN districts ON buildings.district_id = districts.id WHERE dist_name = 'Московский'";
					rc = sqlite3_exec(db, sql, callback, 0, &errMsg);
					if (rc != SQLITE_OK) {
						fprintf(stderr, "SQL error: %s\n", errMsg);
						sqlite3_free(errMsg);
					}
					break;
				}
			case 7:
				{
					break;
				}
            default:
                {
                    printf("Invalid choice.\n");
                    break;
                }
        }
    } while (choice != 7);
    
    sqlite3_close(db);
    return 0;
}
