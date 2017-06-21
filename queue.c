# include "queue.h"

queue *newQueue()
{
	queue *temp = (queue *) malloc(sizeof(queue));
	temp->size = 0;
	temp->head = NULL;
	return temp;
}

void delQueue(queue *inputQueue)
{
	delTree(inputQueue->head);
	free(inputQueue);
}

bool emptyQueue(queue *input)
{
	if(input->size == 0)
	{
		return true;
	}
	
	return false;
}

bool enqueue(queue *inputQueue, treeNode *inputNode)
{
	if(emptyQueue(inputQueue)) // Empty
	{
		inputNode->next = NULL;
		inputQueue->head = inputNode;
		inputQueue->size++;
		return true;
	}
	
	if((inputNode->count < inputQueue->head->count))
	{
		// Smaller than the head
		inputNode->next = inputQueue->head;
		inputQueue->head = inputNode;
		inputQueue->size++;
		return true;
	}

	treeNode *temp = inputQueue->head;
	while(temp->next != NULL)
	{
		if(inputNode->count < (temp->next)->count)
		{
			inputNode->next = temp->next;
			temp->next = inputNode;
			inputQueue->size++;
			return true;
		}

		temp = temp->next;
	}

	temp->next = inputNode;
	inputQueue->size++;
	return true;
}

treeNode *dequeue(queue *inputQueue)
{
	treeNode *node = inputQueue->head;
	if(node == NULL)
	{
		return NULL;
	}
	inputQueue->head = (inputQueue->head)->next;
	inputQueue->size--;
	return node;
}

void printQueue(queue *input)
{
	treeNode *temp = input->head;
	while(temp != NULL)
	{
		printf("%lu\n", (temp->count));
		temp = temp->next;
	}

	printf("size: %d\n", input->size);
}

			
