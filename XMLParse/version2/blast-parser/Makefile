CC=cc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -g
LDFLAGS=
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
TARGET=taxo

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -o $@ -c $< $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(TARGET) $(OBJ)
