CFLAGS = -Wall -Werror -Wextra -pedantic -std=c99 -g
CC = gcc $(CFLAGS)
OBJECTS_E = encode.o stack.o huffman.o queue.o
OBJECTS_D = decode.o stack.o huffman.o queue.o

.PHONY	:	all
all	:	encode decode

encode	:	$(OBJECTS_E)
	$(CC) $(OBJECTS_E) -o encode

decode	:	$(OBJECTS_D)
	$(CC) $(OBJECTS_D) -o decode

encode.o	:	encode.c
	$(CC) -c encode.c

decode.o	:	decode.c
	$(CC) -c decode.c

stack.o	:	stack.c stack.h
	$(CC) -c stack.c

queue.o	:	queue.c	queue.h	huffman.o
	$(CC) -c queue.c

huffman.o	:	huffman.c huffman.h
	$(CC) -c huffman.c

clean	:	
	rm -rf $(OBJECTS_E) encode decode decode.o
