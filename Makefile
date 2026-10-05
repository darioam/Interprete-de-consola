# Compilador
CC = gcc

# Flags de compilación
CFLAGS = -Wall -Wextra -std=c11 -I include

# Nombre del ejecutable
TARGET = main

# Directorio de compilación
BUILD_DIR = build

# Buscar recursivamente archivos fuente en src/
SRC = $(shell find src -type f -name "*.c")

# Generar los nombres de los archivos objeto
OBJ = $(patsubst src/%.c, $(BUILD_DIR)/%.o, $(SRC))

# Regla principal
all: build

# Crear el directorio de compilación si no existe
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compilar
build: $(BUILD_DIR) $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) -lm -ldl 

# Regla para compilar archivos fuente a objetos
$(BUILD_DIR)/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Ejecutar el programa
run: build
	./$(TARGET)

# Limpiar archivos compilados
clean:
	rm -rf $(TARGET) $(BUILD_DIR)
	@echo "Cleaning up..."

