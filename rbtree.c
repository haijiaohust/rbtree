#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

#define _rb_parent(r)	((r)->parent)
#define _rb_color(r)	((r)->color)
#define _rb_is_red(r)	((r)->color == RED)
#define _rb_is_black(r)	((r)->color == BLACK)
#define _rb_set_red(r)	do {(r)->color = RED;} while(0)
#define _rb_set_black(r)	do {(r)->color = BLACK;} while(0)
#define _rb_set_color(r, c)	do {(r)->color = c;} while(0)

static void _preorder(RB_node* node)
{
	if(node){
		printf("%d %d %p %p %p %p\n", node->key, \
			node->color, node, node->parent, node->left, node->right);
		_preorder(node->left);
		_preorder(node->right);
	}
}
void preorder(RB_root* root)
{
	if(root)
		_preorder(root->node);
}
static void _inorder(RB_node* node)
{
	if(node){
		_inorder(node->left);
		printf("%d %d %p %p %p %p\n", node->key, \
			node->color, node, node->parent, node->left, node->right);
		_inorder(node->right);
	}
}
void inorder(RB_root* root)
{
	if(root)
		_inorder(root->node);
}
/*
* search the minimum node with root of parameter node
*/
static RB_node* _min_node(RB_node* node)
{
	RB_node* p = node;
	if(p){
		while(p->left)
			p = p->left;
		return p;
	}
	return NULL;
}
/*
* search the maximum node with root of parameter node
*/
static RB_node* _max_node(RB_node* node)
{
	RB_node* p = node;
	if(p){
		while(p->right)
			p = p->right;
		return p;
	}
	return NULL;
}
static RB_node* successor(RB_node* node)
{
	RB_node* y = NULL;
	if(node->right)
		return _min_node(node->right);
	y = node->parent;
	while(y && node == y->right){
		node = y;
		y = y->parent;
	}
	return y;
}
static RB_node* predecessor(RB_node* node)
{
	RB_node* y = NULL;
	if(node->left)
		return _max_node(node->left);
	y = node->parent;
	while(y && node == y->left){
		node = y;
		y = y->parent;
	}
	return y;
}

static RB_node* RB_create_node(e_type a)
{
	RB_node* p = (RB_node*)malloc(sizeof(RB_node));
	if(p){
		p->key = a;
		p->left = NULL;
		p->right = NULL;
		p->parent = NULL;
		_rb_set_color(p, BLACK);
	}
	return p;
}
static RB_node* _search(RB_node* p, e_type k)
{
	RB_node* q = p;
	if(!p)
		return NULL;
	while(q && q->key != k){
		if(q->key > k)
			q = q->left;
		else q = q->right;
	}
	return q;
}
int RB_search(RB_root* root, e_type k)
{
	return _search(root->node, k) ? 0 : -1;
}
/* 
 * 对红黑树的节点(x)进行左旋转
 *
 * 左旋示意图(对节点x进行左旋)：
 *      px                              px
 *     /                               /
 *    x                               y                
 *   /  \      --(左旋)-->           / \
 *  lx   y                          x  ry     
 *     /   \                       /  \
 *    ly   ry                     lx  ly  
 *
 *
 */
static void _rb_left_rotate(RB_root* root, RB_node* x)
{
	RB_node* y = x->right;
	x->right = y->left;
	if(y->left)
		y->left->parent = x;
	y->parent = x->parent;
	if(!x->parent)		//如果x是根节点
		root->node = y;
	else{
		if(x->parent->left == x)
			x->parent->left = y;
		else x->parent->right = y;
	}
	y->left = x;
	x->parent = y;
}
/* 
 * 对红黑树的节点(y)进行右旋转
 *
 * 右旋示意图(对节点y进行左旋)：
 *            py                               py
 *           /                                /
 *          y                                x                  
 *         /  \      --(右旋)-->            /  \ 
 *        x   ry                           lx   y  
 *       / \                                   / \
 *      lx  rx                                rx  ry
 * 
 */
static void _rb_right_rotate(RB_root* root, RB_node* y)
{
	RB_node* x = y->left;
	y->left = x->right;
	if(x->right)
		x->right->parent = y;
	x->parent = y->parent;
	if(!x->parent)
		root->node = x;
	else{
		if(y == y->parent->left)
			y->parent->left = x;
		else y->parent->right = x;
	}
	x->right = y;
	y->parent = x;
}
static void _insert_fixup(RB_root* root, RB_node* node)
{
	RB_node* parent = NULL;
	RB_node* gparent = NULL;
	// 若“父节点存在，并且父节点的颜色是红色”
	while((parent = _rb_parent(node)) && _rb_is_red(parent)){
		gparent = _rb_parent(parent);
		if(parent == gparent->left){
			// Case 1条件：叔叔节点是红色
			if(gparent->right && _rb_is_red(gparent->right)){
				_rb_set_black(gparent->right);
				_rb_set_black(parent);
				_rb_set_red(gparent);
				node = gparent;
				continue;
			}
			// Case 2条件：叔叔是黑色，且当前节点是右孩子
			if(parent->right == node){
				RB_node* tmp = NULL;
				_rb_left_rotate(root, parent);
				tmp = parent;
				parent = node;
				node = tmp;
			}
			// Case 3条件：叔叔是黑色，且当前节点是左孩子
			_rb_set_black(parent);
			_rb_set_red(gparent);
			_rb_right_rotate(root, gparent);
		}
		else{
			// Case 1条件：叔叔节点是红色
			if(gparent->left && _rb_is_red(gparent->left)){
				_rb_set_black(gparent->left);
				_rb_set_black(parent);
				_rb_set_red(gparent);
				node = gparent;
				continue;
			}
			// Case 2条件：叔叔是黑色，且当前节点是左孩子
			if(parent->left == node){
				RB_node* tmp = NULL;
				_rb_right_rotate(root, parent);
				tmp = parent;
				parent = node;
				node = tmp;
			}
			// Case 3条件：叔叔是黑色，且当前节点是右孩子。
			_rb_set_black(parent);
			_rb_set_red(gparent);
			_rb_left_rotate(root, gparent);
		}
	}
	_rb_set_black(root->node);
}
static void _insert(RB_root* root, RB_node* node)
{
	RB_node* y = root->node;
	RB_node* x = root->node;
	while(x){
		y = x;
		if(x->key > node->key)
			x = x->left;
		else x = x->right;
	}
	if(!y)
		root->node = node;
	else if(y->key > node->key)
		y->left = node;
	else y->right = node;
	node->parent = y;
	_rb_set_red(node);
	_insert_fixup(root, node);
}
int RB_insert(RB_root* root, e_type k)
{
	RB_node* node = NULL;
	if(!RB_search(root, k)){
		printf("key is existed %d\n", k);
		return -1;
	}
	if((node = RB_create_node(k)) == NULL){
		printf("create node error\n");
		return -1;
	}
	_insert(root, node);
	return 0;
}

static void _delete_fixup(RB_root* root, RB_node* node, RB_node* parent)
{
	RB_node* other = NULL;
	while( (!node || _rb_is_black(node)) && node != root->node){
		if(parent->left == node){
			other = parent->right;
			if(_rb_is_red(other)){
				// Case 1: x的兄弟w是红色的  
				_rb_set_black(other);
				_rb_set_red(parent);
				_rb_left_rotate(root, parent);
				other = parent->right;
			}
			if((!other->left || _rb_is_black(other->left)) &&
				(!other->right || _rb_is_black(other->right)))
			{
				// Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的  
				_rb_set_red(other);
				node = parent;
				parent = _rb_parent(node);	
			}
			else{
				if(!other->right || _rb_is_black(other->right)){
					// Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。
					_rb_set_black(other->left);
					_rb_set_red(other);
					_rb_right_rotate(root, other);
					other = parent->right;
				}
				// Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
				_rb_set_color(other, _rb_color(parent));
				_rb_set_black(parent);
				_rb_set_black(other->right);
				_rb_left_rotate(root, parent);
				node = root->node;
				break;
			}
		}
		else{
			other = parent->left;
			if(_rb_is_red(other)){
				// Case 1: x的兄弟w是红色的  
				_rb_set_black(other);
				_rb_set_red(parent);
				_rb_right_rotate(root, parent);
				other = parent->left;
			}
			if((!other->left || _rb_is_black(other->left)) &&
				(!other->right || _rb_is_black(other->right)))
			{
				// Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的
				_rb_set_red(other);
				node = parent;
				parent = _rb_parent(node);
			}
			else{
				if(!other->left || _rb_is_black(other->left)){
					// Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。
					_rb_set_black(other->right);
					_rb_set_red(other);
					_rb_left_rotate(root, other);
					other = parent->left;
				}
				// Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
				_rb_set_color(other, _rb_color(parent));
				_rb_set_black(parent);
				_rb_set_black(other->left);
				_rb_right_rotate(root, parent);
				node = root->node;
				break;
			}
		}
	}
	if(node)
		_rb_set_black(node);
}
static void _delete(RB_root* root, RB_node* node)
{
	RB_node* child = NULL;
	RB_node* parent = NULL;
	unsigned char color;
	if(!node->left)
		child = node->right;
	else if(!node->right)
		child = node->left;
	else{
		//查找node的后继
		RB_node* replace = node->right;
		while(replace->left)
			replace = replace->left;

		if(_rb_parent(node)){
			if(_rb_parent(node)->left == node)
				_rb_parent(node)->left = replace;
			else _rb_parent(node)->right = replace;
		}
		else root->node = replace;

		// child是"取代节点"的右孩子，也是需要"调整的节点"。
        // "取代节点"肯定不存在左孩子！因为它是一个后继节点。
		child = replace->right;
		parent = _rb_parent(replace);
		color = _rb_color(replace);

		// "被删除节点"是"它的后继节点的父节点"
		if(parent == node)
			parent = replace;
		else {
			if(child)
				child->parent = parent;
			parent->left = child;
			replace->right = node->right;
			node->right->parent = replace;
		}

		replace->parent = node->parent;
		replace->color = node->color;
		replace->left = node->left;
		node->left->parent = replace;

		if(color == BLACK)
			goto fixup;
	}

	if(node->left)
		child = node->left;
	else child = node->right;

	parent = node->parent;
	color = node->color;

	if(child)
		child->parent = parent;

	if(parent){
		if(parent->left == node)
			parent->left = child;
		else parent->right = child;
	}
	else root->node = child;

fixup:
	if(color == BLACK)
		_delete_fixup(root, child, parent);
	free(node);
}
void RB_delete(RB_root* root, e_type k)
{
	RB_node* node = _search(root->node, k);
	if(!node){
		printf("can not find k = %d",k);
		return;
	}
	_delete(root, node);
}
RB_root* RB_create()
{
	RB_root* root = (RB_root*)malloc(sizeof(RB_root));
	if(root)
		root->node = NULL;

	return root;
}
void RB_destory(RB_node* p)
{
	if(!p)
		return;
	RB_destory(p->left);
	RB_destory(p->right);
	free(p);
}