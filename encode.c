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
int main(int argc, char **argv)
{
	int c, verbose = 0, isOutput = 0;
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
				isOutput = 1;
				break;
			case 'v':
				verbose = 1;
				break;
		}
	}
	
	if(inputFile == NULL)
	{
		printf("Please specify input file."); 
		exit(1);
	}
	
	struct stat buffer;
	if(stat(outputFile, &buffer) == 0)
	{
		printf("Output file already exists. Exiting.");
		exit(1);
	}
		
	uint32_t histogram[256];
	for(int i = 0; i < 256; i++)
	{
		histogram[i] = 0;
	}
	
	
	int file;
	file = open(inputFile, O_RDONLY);
	
	if(file < 0)
	{
		printf("Input file not opened successfully.\n");	
		exit(errno);
	}
	
	uint8_t buf[1024];
	int bytesRead = 0;
	uint64_t fileLength = 0;
	
	do
	{
		bytesRead = read(file,buf,1024);
		for(int i = 0; i < bytesRead; i++)
		{
			histogram[(buf[i])]++;
			fileLength++;
		}
	} while(bytesRead >= 1024);
	
	bytesRead = 0;
	
	histogram[0]++;
	histogram[255]++;

	queue *pQueue = newQueue();

	// Add nodes into the priority queue

	uint16_t leafCount = 0;
	for(int i = 0; i < 256; i++)
	{
		if(histogram[i] != 0)
		{
			leafCount++;
			enqueue(pQueue, newTreeNode(i, histogram[i], true));
		}
	}
	
	while(pQueue->size > 1)
	{
		treeNode *first = dequeue(pQueue);
		treeNode *second = dequeue(pQueue);
		enqueue(pQueue, join(first, second));
	}	
		
	code codeArray[256];

	for(int i = 0; i < 256; i++)
	{
		codeArray[i] = newCode();
	}

	code tempCode = newCode();
	buildCode(pQueue->head, tempCode, codeArray);
	
	uint32_t magicNumber = 0xdeadd00d;
	
	int o_file = 1;										//By default, ouput is stdout

	// Mode flags from Stackoverflow usr krlmlr
	
	if(isOutput == 1)
	{
		o_file = open(outputFile, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);		//If an ouput is specified, open it
	}
	
	if(o_file < 0)
	{
		printf("Output file not opened.\n");
		exit(errno);
	}

	if(isOutput)
	{
		write(o_file, &magicNumber, sizeof(magicNumber));
		write(o_file, &fileLength, sizeof(fileLength));
	
		uint16_t treeCount = (3 * leafCount) - 1;
		write(o_file, &treeCount, sizeof(treeCount));
	
		dumpTree(pQueue->head, o_file);
	}
	else if(!verbose)
	{
		write(o_file, &magicNumber, sizeof(magicNumber));
                write(o_file, &fileLength, sizeof(fileLength));

                uint16_t treeCount = (3 * leafCount) - 1;
                write(o_file, &treeCount, sizeof(treeCount));

                dumpTree(pQueue->head, o_file);
	}

	for(int i = 0; i < 1024; i++) { buf[i] = 0; }
	lseek(file, 0, 0);
	
	bitV *appender = newVec(fileLength * 8);

	do
	{
		bytesRead = read(file,buf,1024);
		for(int i = 0; i < bytesRead; i++)
		{
			uint64_t tempLength = codeArray[buf[i]].l;
			code tempCode = codeArray[buf[i]]; 			// Dont want to pop from array in case of repeat characters
			uint8_t codeBuffer[256]; 				// Used to write code in correct order (code popped in reverse order)
			for(int k = 0; k < 256; k++) { codeBuffer[k] = 0;} 	// Reset code buffer
			for(int k = tempLength - 1; k >= 0; k--) 		// Write code backwards (right way) into code buffer
			{
				uint32_t tempHolder;
				popCode(&tempCode, &tempHolder);
				codeBuffer[k] = tempHolder;
			}

			// Write code to file
			for(uint64_t k = 0; k < tempLength; k++)
			 {
				if(codeBuffer[k] == 1) 
				{
					setBit(appender, ( (appender->current) + k));
				}

				if(( (appender->current) + k) >= appender->l)
				{
					appender->v = realloc(appender->v, (2 * (appender->l / 8)) * sizeof(uint8_t));  // In case bit vector fills up
					appender->l *= 2;
				}
			 } 
			appender->current = appender->current + tempLength;
		}
	}while(bytesRead >= 1024);

	if(isOutput)
	{
		write(o_file, appender->v, (appender->current / 8) + 1);
		if(verbose)
		{
			printf("Original File Size: %lu, Compressed File Size: %lu\n", fileLength, (appender->current / 8) + 1);
		}
	}
	
	else if (verbose)						// If verbose is turned on, and no output file specified, no binary will be emmitted
	{
		printf("Original File Size: %lu, Compressed File Size: %lu\n", fileLength, (appender->current / 8) + 1);
	}

	else
	{
		// Code is the same, however o_file is STDOUT here
		write(o_file, appender->v, (appender->current / 8) + 1);
	}


	delQueue(pQueue);
	delVec(appender);
	free(inputFile);
	free(outputFile);
	close(file);
	close(o_file);
}
