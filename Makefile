# Makefile for API Gateway Request Scheduler

# Derleyici ve bayraklar
CC = gcc
CFLAGS = -Wall -Wextra -g -fsanitize=address -Iinclude

# Kaynak dosyalar ve çıktı dosyası
SRC = src/main.c src/priority_queue.c src/rfc_parser.c
OBJ = $(SRC:.c=.o)
TARGET = gateway_scheduler

# Varsayılan hedef
all: $(TARGET)

# Derleme hedefi
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

# Temizlik hedefi
clean:
	rm -f $(OBJ) $(TARGET)

# Çalıştırma hedefi
run: $(TARGET)
	./$(TARGET)

# Valgrind ile bellek kontrolü
valgrind: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)