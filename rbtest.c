#include <stdio.h>
#include "rbtree.h"

//#define DEBUG_INSERT
#define DEBUG_DELETE
#define LENGTH(a) ( (sizeof(a)) / (sizeof(a[0])) )
int main()
{
	RB_root* root = RB_create();
	int a[] = {13, 8, 1, 6, 11, 17, 15, 25, 22, 27};
	//int a[] = {10, 40, 30, 60, 90, 70, 20, 50, 80};
	int len = LENGTH(a);
	int i;
	for(i = 0; i < len; i++)
	{
		RB_insert(root, a[i]);
#ifdef DEBUG_INSERT
		printf("insert: %d\n", a[i]);
		printf("inorder:\n");
		inorder(root);
#endif
	}
	printf("preorder:\n");
	preorder(root);
	printf("inorder:\n");
	inorder(root);
	for(i = 0; i < len; i++){
		RB_delete(root, a[i]);
#ifdef DEBUG_DELETE
		printf("delete: %d\n", a[i]);
		printf("inorder\n");
		inorder(root);
#endif
	}
	RB_destory(root->node);

	return 0;
}