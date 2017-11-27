#include <stdlib.h>

#include "queue.h"
#include "node.h"

struct _queue
{
	node * front;
	node * back;
};

static void recursiveDeleteNode(node *);

queue * newQueue()
{
	queue * q = malloc(sizeof(queue));
	if(!q)
		return NULL;
	q->front = NULL;
	q->back = NULL;
	return q;
}

void deleteQueue(queue * q)
{
	recursiveDeleteNode(q->front);
	free(q);
}

void enqueue(queue * q, void * d)
{
	node * n = newNode(1);
	if(!n)
		return;
	setData(n, d);
	if(q->back)
		linkNodeTo(q->back, n, 0);
	else
		q->front = n;
	q->back = n;
}

void * dequeue(queue * q)
{
	if(!q->front)
		return NULL;
	node * n = q->front;
	if(q->front == q->back)
		q->back = NULL;
	q->front = getChild(q->front, 0);
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
