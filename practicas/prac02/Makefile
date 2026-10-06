CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET1 = programa1
TARGET2 = programa2

all: $(TARGET1) $(TARGET2)

$(TARGET1): programa1.c
	$(CC) $(CFLAGS) -o $(TARGET1) programa1.c

$(TARGET2): programa2.c
	$(CC) $(CFLAGS) -o $(TARGET2) programa2.c

clean:
	rm -f $(TARGET1) $(TARGET2) matriz.txt resultados.txt

.PHONY: all clean