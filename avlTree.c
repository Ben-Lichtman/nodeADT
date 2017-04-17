#include <stdlib.h>

#include "avlTree.h"
#include "node.h"

typedef struct _dataContainer dataContainer;

struct _dataContainer
{
	void * data;
	TYPE key;
	unsigned int height;
};

struct _avlTree
{
	node * root;
};

static node * leftRot(node *);
static node * rightRot(node *);
static void recursiveDeleteNode(node *);
static node * insertNode(node * root, node *, TYPE);
static node * removeNode(node * root, TYPE);
static node * searchNode(node *, TYPE);
static int maxSubHeight(node *);
static int getBalance(node *);
static node * rebalance(node * n);
static node * scanLeft(node * n, node ** replacement);
static node * scanRight(node * n, node ** replacement);

avlTree * newAvlTree()
{
	avlTree * t = malloc(sizeof(avlTree));
	if(!t)
		return NULL;
	t->root = NULL;
	return t;
}

void deleteAvlTree(avlTree * t)
{
	recursiveDeleteNode(t->root);
	free(t);
}

void insertAvlTree(avlTree * t, void * d, TYPE key)
{
	node * n = newNode(2);
	if(!n)
		return;
	dataContainer * c = malloc(sizeof(dataContainer));
	c->data = d;
	c->key = key;
	c->height = 1;
	setData(n, c);
	t->root = insertNode(t->root, n, key);
}

void removeAvlTree(avlTree * t, TYPE key)
{
	t->root = removeNode(t->root, key);
}

int searchAvlTree(avlTree * t, TYPE key)
{
	return searchNode(t->root, key) != NULL;
}

void * retrieveAvlTree(avlTree * t, TYPE key)
{
	node * n = searchNode(t->root, key);
	if(!n)
		return NULL;
	return ((dataContainer *)getData(n))->data;
}

static node * leftRot(node * root)
{
	node * newRoot = getChild(root, 1);
	linkNodeTo(root, 1, getChild(newRoot, 0));
	linkNodeTo(newRoot, 0, root);
	((dataContainer *)getData(root))->height = maxSubHeight(root)+1;
	((dataContainer *)getData(newRoot))->height = maxSubHeight(newRoot)+1;
	return newRoot;
}

static node * rightRot(node * root)
{
	node * newRoot = getChild(root, 0);
	linkNodeTo(root, 0, getChild(newRoot, 1));
	linkNodeTo(newRoot, 1, root);
	((dataContainer *)getData(root))->height = maxSubHeight(root)+1;
	((dataContainer *)getData(newRoot))->height = maxSubHeight(newRoot)+1;
	return newRoot;
}

static void recursiveDeleteNode(node * n)
{
	if(!n)
		return;
	recursiveDeleteNode(getChild(n, 0));
	recursiveDeleteNode(getChild(n, 1));
	deleteNode(n);
}

static node * insertNode(node * root, node * n, TYPE key)
{
	if(!root)
		return n;
	dataContainer * c = getData(root);
	if(key < c->key)
		linkNodeTo(root, 0, insertNode(getChild(root, 0), n, key));
	else if(c->key < key)
		linkNodeTo(root, 1, insertNode(getChild(root, 1), n, key));
	else
		//Determine what to do in case of duplicates
		linkNodeTo(root, 0, insertNode(getChild(root, 0), n, key));
	return rebalance(root);
}

static node * removeNode(node * root, TYPE key)
{
	if(!root)
		return NULL;
	dataContainer * c = getData(root);
	if(key < c->key)
		linkNodeTo(root, 0, removeNode(getChild(root, 0), key));
	else if(c->key < key)
		linkNodeTo(root, 1, removeNode(getChild(root, 1), key));
	else
	{
		node * replacement = NULL;
		int balance = getBalance(root);
		if(balance <= 0)
			linkNodeTo(root, 0, scanRight(getChild(root, 0), &replacement));
		else
			linkNodeTo(root, 1, scanLeft(getChild(root, 1), &replacement));
		if(replacement)
		{
			linkNodeTo(replacement, 0, getChild(root, 0));
			linkNodeTo(replacement, 1, getChild(root, 1));
		}
		deleteNode(root);
		return rebalance(replacement);
	}
	return rebalance(root);
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
	else
		return root;
}

static int maxSubHeight(node * n)
{
	int leftHeight = getChild(n, 0)? ((dataContainer *)getData(getChild(n, 0)))->height : 0;
	int rightHeight = getChild(n, 1)? ((dataContainer *)getData(getChild(n, 1)))->height : 0;
	return leftHeight > rightHeight? leftHeight : rightHeight;
}

static int getBalance(node * n)
{
	int leftHeight = getChild(n, 0)? ((dataContainer *)getData(getChild(n, 0)))->height : 0;
	int rightHeight = getChild(n, 1)? ((dataContainer *)getData(getChild(n, 1)))->height : 0;
	return rightHeight - leftHeight;
}

static node * rebalance(node * n)
{
	if(!n)
		return n;
	int balance = getBalance(n);
	if(balance < -1)
	{
		int childBalance = getBalance(getChild(n, 0));
		if(childBalance > 0)
			linkNodeTo(n, 0, leftRot(getChild(n, 0)));
		return rightRot(n);
	}
	else if(balance > 1)
	{
		int childBalance = getBalance(getChild(n, 1));
		if(childBalance < 0)
			linkNodeTo(n, 1, rightRot(getChild(n, 1)));
		return leftRot(n);
	}
	((dataContainer *)getData(n))->height = maxSubHeight(n)+1;
	return n;
}

static node * scanLeft(node * n, node ** replacement)
{
	if(!n)
		return NULL;
	if(getChild(n, 0))
		linkNodeTo(n, 0, scanLeft(getChild(n, 0), replacement));
	else
	{
		*replacement = n;
		return getChild(n, 0);
	}
	return rebalance(n);
}

static node * scanRight(node * n, node ** replacement)
{
	if(!n)
		return NULL;
	if(getChild(n, 1))
		linkNodeTo(n, 1, scanRight(getChild(n, 1), replacement));
	else
	{
		*replacement = n;
		return getChild(n, 1);
	}
	return rebalance(n);
}
