#define TYPE					int

typedef struct _avlTree avlTree;
typedef avlTree * AvlTree;

avlTree * newAvlTree		();

void deleteAvlTree			(avlTree *);

void insertAvlTree			(avlTree *, void *, TYPE key);

void removeAvlTree			(avlTree *, TYPE key);

int searchAvlTree			(avlTree *, TYPE key);

void * retrieveAvlTree		(avlTree *, TYPE key);
