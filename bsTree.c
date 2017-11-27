#include <stdlib.h>

#include "bsTree.h"
#include "node.h"

typedef struct _dataContainer dataContainer;

struct _bsTree
{
	node * root;
	int (*cmp)(void *, void *);
	void (*dealloc)(void *);
};

static void recursiveDeleteNode(node * n, void (*dealloc)(void *));
static node * insertNode(node * root, node *, int (*cmp)(void *, void *));
static node * removeNode(node * root, void * d, int (*cmp)(void *, void *), void (*dealloc)(void *));
static node * searchNode(node *, void *, int (*cmp)(void *, void *));

static node * scanRight(node * n, node ** replacement);

bsTree * newBsTree(int (*cmp)(void * a, void * b), void (*dealloc)(void *))
{
	bsTree * t = malloc(sizeof(bsTree));
	if(!t)
		return NULL;
	t->root = NULL;
	t->cmp = cmp;
	t->dealloc = dealloc;
	return t;
}

void deleteBsTree(bsTree * t)
{
	recursiveDeleteNode(t->root, t->dealloc);
	free(t);
}

void insertBsTree(bsTree * t, void * d)
{
	node * n = newNode(2);
	if(!n)
		return;
	setData(n, d);
	t->root = insertNode(t->root, n, t->cmp);
}

void removeBsTree(bsTree * t, void * d)
{
	t->root = removeNode(t->root, d, t->cmp, t->dealloc);
}

int searchBsTree(bsTree * t, void * d)
{
	return searchNode(t->root, d, t->cmp) != NULL;
}

void * retrieveBsTree(bsTree * t, void * d)
{
	node * n = searchNode(t->root, d, t->cmp);
	if(!n)
		return NULL;
	return getData(n);
}

static void recursiveDeleteNode(node * n, void (*dealloc)(void *))
{
	if(!n)
		return;
	recursiveDeleteNode(getChild(n, 0), dealloc);
	recursiveDeleteNode(getChild(n, 1), dealloc);
	dealloc(getData(n));//Free the given data
	deleteNode(n);//Free the node
}

static node * insertNode(node * root, node * n, int (*cmp)(void *, void *))
{
	if(!root)
		return n;
	int comparison = cmp(getData(root), getData(n));
	if(comparison < 0)
		linkNodeTo(root, insertNode(getChild(root, 0), n, cmp), 0);
	else if(0 < comparison)
		linkNodeTo(root, insertNode(getChild(root, 1), n, cmp), 1);
	else
		//Determine what to do in case of equality
		linkNodeTo(root, insertNode(getChild(root, 0), n, cmp), 0);
	return root;
}

static node * removeNode(node * root, void * d, int (*cmp)(void *, void *), void (*dealloc)(void *))
{
	if(!root)
		return NULL;
	int comparison = cmp(getData(root), d);
	if(comparison < 0)
		linkNodeTo(root, removeNode(getChild(root, 0), d, cmp, dealloc), 0);
	else if(0 < comparison)
		linkNodeTo(root, removeNode(getChild(root, 1), d, cmp, dealloc), 1);
	else
	{
		node * replacement = NULL;
		linkNodeTo(root, scanRight(getChild(root, 0), &replacement), 0);
		if(replacement)
		{
			linkNodeTo(replacement, getChild(root, 0), 0);
			linkNodeTo(replacement, getChild(root, 1), 1);
		}
		dealloc(getData(root));//Free the given data
		deleteNode(root);//Free the node
		return replacement;
	}
	return root;
}

static node * searchNode(node * root, void * d, int (*cmp)(void *, void *))
{
	if(!root)
		return NULL;
	int comparison = cmp(getData(root), d);
	if(comparison < 0)
		return searchNode(getChild(root, 0), d, cmp);
	else if(0 < comparison)
		return searchNode(getChild(root, 1), d, cmp);
	return root;
}

static node * scanRight(node * n, node ** replacement)
{
	if(!n)
		return NULL;
	node * child = getChild(n, 1);
	if(child)
		linkNodeTo(n, scanRight(child, replacement), 1);
	else
	{
		*replacement = n;
		return getChild(n, 0);
	}
	return n;
}