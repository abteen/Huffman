# ifndef _CODE_H
# define _CODE_H

# include <stdint.h>
# include <stdbool.h>

/**
 * 	This code is from the assigment PDF
**/


typedef struct code
{
	uint8_t bits[32];
	uint32_t l;
} code;

static inline code newCode()
{
	code t;
	for (int i = 0; i < 32; i += 1) { t.bits[i] = 0; }
	t.l = 0;
	return t;
}

static inline bool pushCode(code *c, uint32_t k)
{
	if (c->l > 256)
	{
		return false;
	}

	else if (k == 0)
	{
		c->bits[c->l / 8] &= ~(0x1 << (c->l % 8));
		c->l += 1;
	}
	
	else
	{
		c->bits[c->l / 8] |= (0x1 << (c->l % 8));
		c->l += 1;
	}

	return true;
}

static inline bool popCode(code *c, uint32_t *k)
{
	if (c->l == 0)
	{
		return false;
	}
		
	else
	{
		c->l -= 1;
		*k = ((0x1 << (c->l % 8)) & c->bits[c->l / 8]) >> (c->l % 8);
		return true;
	}
}

static inline bool emptyCode(code *c) {return c->l == 0; }

static inline bool fullCode(code *c) { return c->l == 256; }

static inline void printCode(code c)
{
	uint32_t temp = 0;
	for(uint32_t i = 0; i < 10; i++)
	{
		popCode(&c, &temp);
		printf("%d", temp);
	}
	printf("popcode length: %d",c.l);
	printf("\n");
}
# endif
