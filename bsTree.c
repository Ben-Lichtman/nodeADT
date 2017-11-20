#include <stdlib.h>

#include "bsTree.h"
#include "node.h"

typedef struct _dataContainer dataContainer;

struct _dataContainer
{
	void * data;
	TYPE key;
};

struct _bsTree
{
	node * root;
};

static void recursiveDeleteNode(node * n);
static node * insertNode(node * root, node *);
static node * removeNode(node * root, TYPE key);
static node * searchNode(node * root, TYPE key);
static node * scanLeft(node * n, node ** replacement);
static node * scanRight(node * n, node ** replacement);

bsTree * newBsTree()
{
	bsTree * t = malloc(sizeof(bsTree));
	if(!t)
		return NULL;
	t->root = NULL;
	return t;
}

void deleteBsTree(bsTree * t)
{
	recursiveDeleteNode(t->root);
	free(t);
}

void insertBsTree(bsTree * t, void * d, TYPE key)
{
	node * n = newNode(2);
	if(!n)
		return;
	dataContainer * c = malloc(sizeof(dataContainer));
	c->data = d;
	c->key = key;
	setData(n, c);
	insertNode(t->root, n);
}

void removeBsTree(bsTree * t, TYPE key)
{
	t->root = removeNode(t->root, key);
}

int searchBsTree(bsTree * t, TYPE key)
{
	return searchNode(t->root, key) != NULL;
}

void * retrieveBsTree(bsTree * t, TYPE key)
{
	node * n = searchNode(t->root, key);
	if(!n)
		return NULL;
	return ((dataContainer *)getData(n))->data;
}

static void recursiveDeleteNode(node * n)
{
	if(!n)
		return;
	recursiveDeleteNode(getChild(n, 0));
	recursiveDeleteNode(getChild(n, 1));
	free((dataContainer *)getData(n));
	deleteNode(n);
}

static node * insertNode(node * root, node * n)
{
	if(!root)
		return n;
	dataContainer * rc = getData(root);
	dataContainer * nc = getData(n);
	if(nc->key < rc->key)
		linkNodeTo(root, insertNode(getChild(root, 0), n), 0);
	else if(rc->key < nc->key)
		linkNodeTo(root, insertNode(getChild(root, 1), n), 1);
	else
		//Determine what to do in case of duplicates
		linkNodeTo(root, insertNode(getChild(root, 0), n), 0);
	return root;
}

static node * removeNode(node * root, TYPE key)
{
	if(!root)
		return NULL;
	dataContainer * c = getData(root);
	if(key < c->key)
		linkNodeTo(root, removeNode(getChild(root, 0), key), 0);
	else if(c->key < key)
		linkNodeTo(root, removeNode(getChild(root, 1), key), 1);
	else
	{
		node * replacement = NULL;
		linkNodeTo(root, scanRight(getChild(root, 0), &replacement), 0);
		if(replacement)
		{
			linkNodeTo(replacement, getChild(root, 0), 0);
			linkNodeTo(replacement, getChild(root, 1), 1);
		}
		free(getData(root));
		deleteNode(root);
		return replacement;
	}
	return root;
}

static node * searchNode(node * root, TYPE key)
{
	if(!root)
		return NULL;
	dataContainer * c = getData(root);
	if(key < c->key)
		return searchNode(getChild(root, 0), key);
	else if(c->key < key)
		return searchNode(getChild(root, 1), key);
	return root;
}

static node * scanLeft(node * n, node ** replacement)
{
	if(!n)
		return NULL;
	if(getChild(n, 0))
		linkNodeTo(n, scanLeft(getChild(n, 0), replacement), 0);
	else
	{
		*replacement = n;
		return getChild(n, 0);
	}
	return n;
}

static node * scanRight(node * n, node ** replacement)
{
	if(!n)
		return NULL;
	if(getChild(n, 1))
		linkNodeTo(n, scanRight(getChild(n, 1), replacement), 1);
	else
	{
		*replacement = n;
		return getChild(n, 1);
	}
	return n;
}