CC = gcc

CFLAGS = -Wall -O2 -std=c11

SRC = testecmdunity.c unity.c cmdproc.c

OBJ = testecmdunity.o unity.o cmdproc.o

PROGRAM = testecmdunity

$(PROGRAM): $(OBJ)
	$(CC) $(CFLAGS) -o $(PROGRAM) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ)

cleanall:
	rm -f $(PROGRAM) $(OBJ)