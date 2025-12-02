CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = ipc_msgqueue
SRC = ipc_msgqueue.c
OBJ = $(SRC:.c=.o)

.PHONY: all run clean help

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	@echo "═══════════════════════════════════════════════════════════"
	@echo "Ejecutando demostración de IPC con System V Message Queues"
	@echo "═══════════════════════════════════════════════════════════"
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

help:
	@echo "Comandos disponibles:"
	@echo "  make        - Compila el programa"
	@echo "  make run    - Compila y ejecuta el programa"
	@echo "  make clean  - Elimina binarios y objetos"
	@echo "  make help   - Muestra este mensaje"
