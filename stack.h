# ifndef _STACK_H
# define _STACK_H
# include <stdint.h>
# include <stdbool.h>
# include <stdlib.h>
# include "huffman.h"

typedef struct stack stack;

struct stack
{
	uint32_t size;
	uint32_t top;
	treeNode **list;
};

stack *newStack();
void delStack(stack *);

treeNode *pop(stack *);
void push(stack *, treeNode *);

bool empty(stack *);
bool full(stack *);
# endif
