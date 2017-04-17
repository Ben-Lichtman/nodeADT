#include <stdlib.h>

#include "node.h"

struct _node
{
	void * data;//Points to a data container
	unsigned int children;
	node ** child;
};

node * newNode(unsigned int children)
{
	node * n = malloc(sizeof(node));
	if(!n)
		return NULL;
	n->data = NULL;
	n->children = children;
	n->child = calloc(sizeof(node *), children);
	return n;
}

void setData(node * n, void * data)
{
	n->data = data;
}

void * getData(node * n)
{
	if(!n)
		return NULL;
	return n->data;
}

node * getChild(node * n, unsigned int index)
{
	if(!n || index >= n->children)
		return NULL;
	return n->child[index];
}

void deleteNode(node * n)
{
	free(n->child);
	free(n);
}

void linkNodeTo(node * n, unsigned int index, node * to)
{
	n->child[index] = to;
}

void unlinkNodeFrom(node * n, unsigned int index)
{
	n->child[index] = NULL;
}
