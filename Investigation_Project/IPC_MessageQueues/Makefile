CC = gcc
CFLAGS = -Wall -Wextra -std=c99

SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

TARGET := $(BIN_DIR)/ipc_msgqueue

.PHONY: all run clean help

all: $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

run: all
	@echo "══════════════════════════════════════════════════════════="
	@echo "Ejecutando demostración de IPC con System V Message Queues"
	@echo "══════════════════════════════════════════════════════════="
	$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

help:
	@echo "Comandos disponibles:"
	@echo "  make        - Compila el programa"
	@echo "  make run    - Compila y ejecuta el programa"
	@echo "  make clean  - Elimina binarios y objetos"
	@echo "  make help   - Muestra este mensaje"
