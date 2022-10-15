# Declaratiile de variabile
CC = gcc
CFLAGS = -g -Wall -lm -std=c11
 
# Regula de compilare
all: build

build: main.c
	$(CC) main.c header.c -o quadtree $(CFLAGS) 

# Regulile de "curatenie" (se folosesc pentru stergerea fisierelor intermediare si/sau rezultate)
.PHONY : clean
clean :
	rm -f quadtree
	rm -f *.out
