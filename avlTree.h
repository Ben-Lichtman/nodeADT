typedef struct _avlTree avlTree;
typedef avlTree * AvlTree;

avlTree * newAvlTree		(int (*cmp)(void *, void *), void (*dealloc)(void *));

void deleteAvlTree			(avlTree *);

void insertAvlTree			(avlTree *, void *);

void removeAvlTree			(avlTree *, void *);

int searchAvlTree			(avlTree *, void *);

void * retrieveAvlTree		(avlTree *, void *);
