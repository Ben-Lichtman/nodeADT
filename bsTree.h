typedef struct _bsTree bsTree;
typedef bsTree * BsTree;

bsTree * newBsTree			(int (*cmp)(void *, void *), void (*dealloc)(void *));

void deleteBsTree			(bsTree *);

void insertBsTree			(bsTree *, void *);

void removeBsTree			(bsTree *, void *);

int searchBsTree			(bsTree *, void *);

void * retrieveBsTree		(bsTree *, void *);
