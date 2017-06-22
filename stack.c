# include "stack.h"

stack *newStack()
{
	stack *tempStack = malloc(sizeof(stack));
	tempStack->size = 10;
	tempStack->top = 0;
	tempStack->list = malloc(tempStack->size * sizeof(treeNode *));
	return tempStack;
}

void delStack(stack *input)
{
	for(uint32_t i = 0; i < input->size; i++)
	{
		delTree(input->list[i]);
	}
	
	free(input);
}

void push(stack *inputStack, treeNode *inputNode)
{
	if(full(inputStack))
	{
		inputStack->size = inputStack->size * 2;
		inputStack->list = realloc(inputStack->list, inputStack->size * sizeof(treeNode *));
	}
	
	inputStack->list[inputStack->top] = inputNode;
	inputStack->top++;
}

treeNode *pop(stack *input)
{
	if(empty(input))
	{
		return NULL;
	}
	treeNode *temp = input->list[input->top - 1];
	input->list[input->top - 1] = NULL;
	input->top--;
	return temp;
}

bool empty(stack *input)
{
	return (input->size == 0);
}

bool full(stack *input)
{
	return (input->size == input->top);
}
