EXEC=parsing


DEP= 

all: $(EXEC)

CC=gcc

CFLAGS=-Wall 

LDFLAGS=  -lexpat


%.o: %.c 

	$(CC) -c $^ $(CFLAGS)


$(EXEC): %: %.c $(DEP)

	$(CC) $(CFLAGS) -o $@  $^ $(LDFLAGS)


clean:

	rm -rf *.o 


mrproper: clean

	rm -rf $(EXEC) 



