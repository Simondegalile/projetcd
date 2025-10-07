# Makefile pour le projet BigBinary

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I.
TARGET = bigbinary_test
OBJS = main.o bigbinary.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c bigbinary.h
	$(CC) $(CFLAGS) -c main.c

bigbinary.o: bigbinary.c bigbinary.h
	$(CC) $(CFLAGS) -c bigbinary.c

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
