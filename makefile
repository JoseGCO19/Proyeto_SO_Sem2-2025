# Makefile para compilar archivos en el directorio actual
# Nombre del ejecutable
TARGET = RUN

# Bibliotecas incluidas, por ejemplo -lm para math.h, -lpthread para hilos
LIBS = -lm -lpthread

# Compilador utilizado
CC = gcc

# Banderas del compilador
CFLAGS = -g -Wall

# Palabras que usa el Makefile que podrían ser el nombre de un programa
.PHONY: default all clean cleanall

# Compilación por defecto
default: $(TARGET)
all: default

# Incluye los archivos .c que están en el directorio actual
SOURCES = $(wildcard *.c)
# Genera los nombres de los archivos .o a partir de los .c
OBJECTS = $(SOURCES:.c=.o)
# Incluye los archivos .h que están en el directorio actual
HEADERS = $(wildcard *.h)

# Compila automáticamente solo archivos fuente que se han modificado
# $< es el primer prerrequisito (el archivo fuente)
# $@ es el nombre del archivo que se está generando (archivo objeto)
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Preserva archivos intermedios
.PRECIOUS: $(TARGET) $(OBJECTS)

# Enlaza objetos y crea el ejecutable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@

# Borra archivos .o
clean:
	rm -f *.o $(TARGET) core

# Borra archivos .o y el ejecutable
cleanall: clean
	rm -f $(TARGET)

# Muestra información de depuración
info:
	@echo "SOURCES: $(SOURCES)"
	@echo "OBJECTS: $(OBJECTS)"
	@echo "HEADERS: $(HEADERS)"