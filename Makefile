# Компилятор
CC = gcc

# Флаги компилятора
CFLAGS = -Wall -Wextra

# Исходные файлы
SRC = main.c functions.c sqlite3.c

# Название исполняемого файла
TARGET = main.exe

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	del /f $(TARGET)
