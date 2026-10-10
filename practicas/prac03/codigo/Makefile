CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pthread

TARGET = programa

all: $(TARGET)

$(TARGET): programa.c
	$(CC) $(CFLAGS) -o $(TARGET) programa.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) a.out

.PHONY: all run clean
