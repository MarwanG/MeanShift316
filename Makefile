SOURCES = calcul.c  tools.c main.c 
OBJ = $(SOURCES:.c=.o)
CC = gcc

all:main

main: $(OBJ)
	$(CC) -o $@ $(OBJ) -linrimage  -lm

%.o: %.c
	$(CC) -c $<  -linrimage -lm

clean:
	rm -f $(OBJ) main

PHONY: all clean