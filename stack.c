#include <stdlib.h>

#include "stack.h"
#include "node.h"

struct _stack
{
	node * top;
};

static void recursiveDeleteNode(node *);

stack * newStack()
{
	stack * s = malloc(sizeof(stack));
	if(!s)
		return NULL;
	s->top = NULL;
	return s;
}

void deleteStack(stack * s)
{
	recursiveDeleteNode(s->top);
	free(s);
}

void push(stack * s, void * d)
{
	node * n = newNode(1);
	if(!n)
		return;
	setData(n, d);
	linkNodeTo(n, s->top, 0);
	s->top = n;
}

void * pop(stack * s)
{
	if(!s->top)
		return NULL;
	node * n = s->top;
	s->top = getChild(s->top, 0);
	void * d = getData(n);
	deleteNode(n);
	return d;
}

static void recursiveDeleteNode(node * n)
{
	if(!n)
		return;
	recursiveDeleteNode(getChild(n, 0));
	deleteNode(n);
}
