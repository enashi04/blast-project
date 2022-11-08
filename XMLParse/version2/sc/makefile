CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra
LDFLAGS=-lexpat
SRC=$(wildcard ./*.c)
OBJ=$(SRC:.c=.o)
TARGET=parsing

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -o $@ -c $< $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(TARGET) $(OBJ)
