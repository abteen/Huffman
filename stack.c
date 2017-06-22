# include "stack.h"
//tester comment
stack *newStack()
{
	stack *tempStack = (stack *) malloc(sizeof(stack));
	tempStack->size = 0;
	tempStack->head = NULL;
	return tempStack;
}

void delStack(stack *input)
{
	delTree(input->head);
	free(input);
}

void push(stack *inputStack, treeNode *inputNode)
{
	treeNode *tempHead = inputStack->head;
	inputStack->head = inputNode;
	inputNode->next = tempHead;
	inputStack->size++;
}

treeNode *pop(stack *input)
{
	if(empty(input))
	{
		return NULL;
	}
	treeNode *temp = input->head;
	input->head = input->head->next;
	input->size--;
	return temp;
}

bool empty(stack *input)
{
	return (input->size == 0);
}
