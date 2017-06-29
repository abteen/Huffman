# ifndef _QUEUE_H
# define _QUEUE_H

# include <stdint.h>
# include <stdbool.h>
# include <stdio.h>

# include "huffman.h"

typedef struct queue
{
	uint32_t size;
	uint32_t head, tail;
	treeNode **array;
}queue;

queue *newQueue();
void delQueue(queue *);

bool emptyQueue(queue *);
bool fullQueue(queue *);

bool enqueue(queue *, treeNode *);
treeNode *dequeue(queue *);

void printQueue(queue *);
 
# endif
