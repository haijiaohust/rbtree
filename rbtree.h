#ifndef _RB_TREE_H_
#define _RB_TREE_H_

#define RED 	0
#define BLACK 	1

typedef int e_type;
typedef struct RBTreeNode{
	unsigned char color;
	e_type key;
	struct RBTreeNode* left;
	struct RBTreeNode* right;
	struct RBTreeNode* parent;
}RB_node;

typedef struct RBTree_root{
	struct RBTreeNode* node;
}RB_root;

RB_root* RB_create();
void RB_destory(RB_node*);
int RB_insert(RB_root*, e_type);
void RB_delete(RB_root*, e_type);
int RB_search(RB_root*, e_type);

#endif