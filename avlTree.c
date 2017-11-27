#include <stdlib.h>

#include "avlTree.h"
#include "node.h"

typedef struct _dataContainer dataContainer;

struct _avlTree
{
	node * root;
	int (*cmp)(void *, void *);
	void (*dealloc)(void *);
};

static void recursiveDeleteNode(node * n, void (*dealloc)(void *));
static node * insertNode(node * root, node *, int (*cmp)(void *, void *));
static node * removeNode(node * root, void * d, int (*cmp)(void *, void *), void (*dealloc)(void *));
static node * searchNode(node *, void *, int (*cmp)(void *, void *));

static node * leftRot(node * root);
static node * rightRot(node * root);
static int height(node * n);
static int balance(node * n);
static node * rebalance(node * n);
static node * scanLeft(node * n, node ** replacement);
static node * scanRight(node * n, node ** replacement);

avlTree * newAvlTree(int (*cmp)(void * a, void * b), void (*dealloc)(void *))
{
	avlTree * t = malloc(sizeof(avlTree));
	if(!t)
		return NULL;
	t->root = NULL;
	t->cmp = cmp;
	t->dealloc = dealloc;
	return t;
}

void deleteAvlTree(avlTree * t)
{
	recursiveDeleteNode(t->root, t->dealloc);
	free(t);
}

void insertAvlTree(avlTree * t, void * d)
{
	node * n = newNode(2);
	if(!n)
		return;
	setData(n, d);
	t->root = insertNode(t->root, n, t->cmp);
}

void removeAvlTree(avlTree * t, void * d)
{
	t->root = removeNode(t->root, d, t->cmp, t->dealloc);
}

int searchAvlTree(avlTree * t, void * d)
{
	return searchNode(t->root, d, t->cmp) != NULL;
}

void * retrieveAvlTree(avlTree * t, void * d)
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
	return rebalance(root);
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
		int bal = balance(root);
		if(bal < 0)
			linkNodeTo(root, scanRight(getChild(root, 0), &replacement), 0);
		else if(bal > 0)
			linkNodeTo(root, scanLeft(getChild(root, 1), &replacement), 1);
		else
			//Default behaviour if balanced
			linkNodeTo(root, scanRight(getChild(root, 0), &replacement), 0);
		if(replacement)
		{
			linkNodeTo(replacement, getChild(root, 0), 0);
			linkNodeTo(replacement, getChild(root, 1), 1);
		}
		dealloc(getData(root));//Free the given data
		deleteNode(root);//Free the node
		return rebalance(replacement);
	}
	return rebalance(root);
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

static node * leftRot(node * root)
{
	node * newRoot = getChild(root, 1);
	linkNodeTo(root, getChild(newRoot, 0), 1);
	linkNodeTo(newRoot, root, 0);
	return newRoot;
}

static node * rightRot(node * root)
{
	node * newRoot = getChild(root, 0);
	linkNodeTo(root, getChild(newRoot, 1), 0);
	linkNodeTo(newRoot, root, 1);
	return newRoot;
}

static int height(node * n)
{
	if(!n)
		return 0;
	int left = height(getChild(n, 0));
	int right = height(getChild(n, 1));
	return left > right ? left + 1 : right + 1;
}

static int balance(node * n)
{
	if(!n)
		return 0;
	int left = height(getChild(n, 0));
	int right = height(getChild(n, 1));
	return right - left;
}

static node * rebalance(node * n)
{
	if(!n)
		return n;
	int bal = balance(n);
	if(bal < -1)
	{
		int childBalance = balance(getChild(n, 0));
		if(childBalance > 0)
			linkNodeTo(n, leftRot(getChild(n, 0)), 0);
		return rightRot(n);
	}
	else if(bal > 1)
	{
		int childBalance = balance(getChild(n, 1));
		if(childBalance < 0)
			linkNodeTo(n, rightRot(getChild(n, 1)), 1);
		return leftRot(n);
	}
	return n;
}

static node * scanLeft(node * n, node ** replacement)
{
	if(!n)
		return NULL;
	node * child = getChild(n, 0);
	if(child)
		linkNodeTo(n, scanLeft(child, replacement), 0);
	else
	{
		*replacement = n;
		return getChild(n, 1);
	}
	return rebalance(n);
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
	return rebalance(n);
}
