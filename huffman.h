# ifndef _HUFFMAN_H
# define _HUFFMAN_H

# include <stdint.h>
# include <stdbool.h>
# include <stdlib.h>
# include <stdio.h>
# include <ctype.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include "code.h"


typedef struct DAH treeNode;

struct DAH
{
	uint8_t symbol;
	uint64_t count;
	bool leaf;
	treeNode *left, *right, *next;
};

treeNode *newTreeNode(uint8_t, uint64_t, bool);
void delTree(treeNode *t);
	
treeNode *join(treeNode *, treeNode *);
void printTree(treeNode *, int);
void printTreeIO(treeNode *, int);

void buildCode(treeNode *, code s, code[]);
void dumpTree(treeNode *, int );


static inline void spaces(int c) { for (int i = 0; i < c; i += 1) { putchar(' '); } return; }
# endif
