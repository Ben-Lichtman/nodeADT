typedef struct _stack stack;
typedef stack * Stack;

stack * newStack			();

void deleteStack			(stack *);

void push					(stack *, void *);

void * pop					(stack *);
