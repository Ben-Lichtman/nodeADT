typedef struct _node node;
typedef node * Node;

node * newNode				(unsigned int children);

void setData				(node *, void * data);

void * getData				(node *);

node * getChild				(node *, unsigned int index);

void deleteNode				(node *);

void linkNodeTo				(node *, unsigned int index, node * to);

void unlinkNodeFrom			(node *, int unsigned index);
