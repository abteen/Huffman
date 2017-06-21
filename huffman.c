# include "huffman.h"

treeNode *newTreeNode(uint8_t s, uint64_t c, bool l)
{
	treeNode *tempNode = (treeNode *) malloc(sizeof(treeNode));
	tempNode->symbol = s;
	tempNode->count = c;
	tempNode->leaf = l;
	tempNode->next = NULL;
	tempNode->left = NULL;
	tempNode->right = NULL;
	return tempNode;
}

void delTree(treeNode *input)
{
	if(input != NULL)
	{
		treeNode *tempLeft = input->left;
		treeNode *tempRight = input->right;
		free(input);
		delTree(tempLeft);
		delTree(tempRight);
	}
}

treeNode *join(treeNode *l, treeNode *r)
{
	int count;
	if(l == NULL || r == NULL)
	{
		count = 0;
	}
	else { count = l->count + r->count; }
	treeNode *temp = newTreeNode('$',count, false);	
	temp->left = l;
	temp->right = r;
	return temp;
}

void printTree(treeNode *t, int depth) // This code from Prof.Long.
{
        if (t && t->leaf)
        {
                if (isalnum(t->symbol))
                {
                        spaces(4 * depth); printf("%c (%lu)\n", t->symbol, t->count);
                }
                else
                {
                        spaces(4 * depth); printf("%X (%lu)\n", t->symbol, t->count);
                }
        }
        else if (t)
        {
                spaces(4 * depth); printf("$ (%lu)\n", t->count);
                printTree(t->left, depth + 1);
                printTree(t->right, depth + 1);
        }
        return;
}

void printTreeIO(treeNode *t, int depth) // This code from Prof. Long.
{
	if (t) 
	{
		printTree(t->left, depth + 1);

		if (t->leaf)
		{
			if (isalnum(t->symbol))
			{
				spaces(4 * depth); printf("'%c' (%lu)\n", t->symbol, t->count);
			}
			else
			{
			spaces(4 * depth); printf("0x%X (%lu)\n", t->symbol, t->count);
			}
		}
		else
		{
			spaces(4 * depth); printf("$ (%lu)\n", t->count);
		}

		printTree(t->right, depth + 1); 
	}

	return;
}
void buildCode(treeNode *t, code s, code table[256])
{
	if(t != NULL)
	{
		if(t->leaf)
		{
			table[t->symbol] = s;
		}

		else
		{
			pushCode(&s, 0);
			buildCode(t->left, s, table);
			uint32_t k = 0;
			popCode(&s, &k);
			pushCode(&s, 1);
			buildCode(t->right, s, table);
			popCode(&s, &k);
		}
	}
}

void dumpTree(treeNode *t, int file)
{
	if(t != NULL)
	{
		if(t->leaf)
		{
			char leaf = 'L';
			uint8_t temp = t->symbol;
			write(file, &leaf, sizeof(leaf));
			write(file, &temp, sizeof(temp));
		}
		
		else
		{
			char internal = 'I';
			dumpTree(t->left, file);
			dumpTree(t->right, file);
			write(file, &internal, sizeof(internal));
		}
	}
}	
