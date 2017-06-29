# include "queue.h"

queue *newQueue()
{
	queue *temp = (queue *) malloc(sizeof(queue));
	temp->size = 10;
	temp->head = 0;
	temp->tail = 0;
	temp->array = malloc(temp->size * sizeof(treeNode *));
	return temp;
}

void delQueue(queue *inputQueue)
{
	for (uint32_t i = inputQueue->head; i != inputQueue->tail; i = (i + 1) % inputQueue->size)
	{
		delTree(inputQueue->array[i]);
	}
	free(inputQueue);
}

bool emptyQueue(queue *input)
{
	return (input->tail == input->head);
}

bool fullQueue(queue *input)
{
	return ( ( (input->tail + 1) % input->size) == input->head );
}

bool enqueue(queue *inputQueue, treeNode *inputNode)
{
	if(fullQueue(inputQueue))
	{
		inputQueue->size = inputQueue->size * 2;
		inputQueue->array = realloc(inputQueue->array, inputQueue->size * sizeof(treeNode *));
	}

	if(emptyQueue(inputQueue))
	{
		inputQueue->array[inputQueue->tail] = inputNode;
		inputQueue->tail = (inputQueue->tail + 1) % inputQueue->size;
		return true;
	}

	uint32_t location = inputQueue->tail;
	if(location == 0) { location = inputQueue->size - 1; }
	else { location--; }
	while((inputQueue->array[location])->count > inputNode->count)
	{
		inputQueue->array[(location + 1) % inputQueue->size] = inputQueue->array[location];
		if(location == inputQueue->head)
		{
			if(inputNode->count > (inputQueue->array[inputQueue->head])->count) { break; }
			else 
			{
				inputQueue->array[inputQueue->head] = inputNode;
				inputQueue->tail = (inputQueue->tail + 1) % inputQueue->size;
				return true;	
			}
		}
		if(location == 0) { location = inputQueue->size - 1; }
		else { location--; }
	}
	inputQueue->array[(location + 1) % inputQueue->size] = inputNode;
	inputQueue->tail = (inputQueue->tail + 1) % inputQueue->size;
	return true;
}

treeNode *dequeue(queue *inputQueue)
{
	if(emptyQueue(inputQueue)) { return NULL; }
	treeNode *node = inputQueue->array[inputQueue->head];
	if(node == NULL)
	{
		return NULL;
	}
	inputQueue->head = (inputQueue->head + 1) % inputQueue->size;
	return node;
}


void printQueue(queue *input)
{
	for(uint32_t i = input->head; i != input->tail; i = (i + 1) % input->size)
	{
		printf("%lu\n", (input->array[i])->count);
	}
}

			
