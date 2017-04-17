typedef struct _queue queue;
typedef queue * Queue;

queue * newQueue			();

void deleteQueue			(queue *);

void enqueue				(queue *, void *);

void * dequeue				(queue *);
