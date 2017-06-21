# include <stdio.h>
# include <string.h>
# include <getopt.h>
# include <stdlib.h>
# include <stdint.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <errno.h>

# include "stack.h"
# include "queue.h"
# include "code.h"
# include "bv.h"

treeNode *loadTree(uint8_t[], uint16_t);

int main(int argc, char ** argv)
{
	int c, verbose = 0, writeVal = 0;
	char *inputFile = NULL, *outputFile = NULL;
	while( (c = getopt(argc, argv, "i:o:v")) != -1)
	{
		switch(c)
		{
			case 'i':
				inputFile = malloc((strlen(optarg) + 1) * sizeof(char));
				strcpy(inputFile, optarg);
				break;
			case 'o':
				outputFile = malloc((strlen(optarg) + 1) * sizeof(char));
				strcpy(outputFile, optarg);
				writeVal = 1;
				break;
			case 'v':
				verbose = 1;
				break;
		}
	}

	int i_file = open(inputFile, O_RDONLY);
	
	uint32_t magicNumber = 0;
	read(i_file, &magicNumber, sizeof(uint32_t));

	if(magicNumber != 0xdeadd00d)
	{
		printf("Source file is not valid.");
		exit(1);
	}
	
	uint64_t origFileSize = 0;
	read(i_file, &origFileSize, sizeof(uint64_t));

	uint16_t treeSize = 0;
	read(i_file, &treeSize, sizeof(uint16_t));

	uint8_t savedTree[treeSize];
	read(i_file, savedTree, treeSize);
	
	treeNode *loadedTree = loadTree(savedTree, treeSize);	
		
	bitV *collector = newVec(origFileSize * 8);
	
	for(uint64_t i = 0; i < origFileSize; i++)
	{
		//printf("i: %lu\n", i);
		read(i_file, (collector->v) + i, sizeof(uint8_t));
	}
	printf("reached\n");	
	int o_file;	

	// If an output is specified, check for existence and then create it, or exit

	if(writeVal == 1)
	{
		
		struct stat buffer; 	// From stackoverflow user codebunny
		if(stat(outputFile, &buffer) == 0)
		{
			printf("Output file already exists. Exiting.\n");
			exit(1);
		}
		
		// Mode flags from stackoverflow user krlmlr
		o_file = open(outputFile, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
		
		if(o_file < 0) 
		{
			printf("Output file creation failed.");
			exit(errno);
		}
	}

	// bvCount keeps track of the bit vector, writeVal and verbose conditionals for writing to screen/output and verbose option

	uint64_t bvCount = 0;
	for(uint64_t i = 0; i < origFileSize; i++)
	{
		treeNode *current = loadedTree;
	
		while(current->leaf == false)
		{
			if(valBit(collector,bvCount) == 1)
			{
				current = current->right;
				bvCount++;
			}
			
			else if(valBit(collector,bvCount) == 0)
			{
				current = current->left;
				bvCount++;
			}
		}

		char c = current->symbol;
		if(writeVal == 0 && verbose == 0)
		{
			write(1, &c, sizeof(char));
		}
		else if (writeVal == 1) 
		{
			write(o_file, &c, sizeof(char));
		}	
	}	

	if(verbose)
	{
		printf("Original File size: %lu\nTree size: %d\n", origFileSize, treeSize); 
	}	

	delTree(loadedTree);	
	delVec(collector);
	close(i_file);
	close(o_file);
	free(inputFile);
	free(outputFile);
		
}

treeNode *loadTree(uint8_t savedTree[], uint16_t treeBytes)
{
        stack *x = newStack();
        for(uint64_t i = 0; i < treeBytes; i++)
        {
                if(savedTree[i] == 'L')
                {
                        treeNode *leaf1 = newTreeNode(savedTree[i+1], 0, true);	
			push(x, leaf1);
			i++;
                }

                else if(savedTree[i] == 'I')
                {
                        treeNode *right = pop(x);
                        treeNode *left = pop(x);
			treeNode *joined = join(left,right);
                        push(x,joined);
                }
        } 
        treeNode *returner = pop(x);
	delStack(x);
	return returner;
}	
