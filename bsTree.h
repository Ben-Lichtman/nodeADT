#define TYPE				int

typedef struct _bsTree bsTree;
typedef bsTree * BsTree;

bsTree * newBsTree			();

void deleteBsTree			(bsTree *);

void insertBsTree			(bsTree *, void *, TYPE key);

void removeBsTree			(bsTree *, TYPE key);

int searchBsTree			(bsTree *, TYPE key);

void * retrieveBsTree		(bsTree *, TYPE key);
