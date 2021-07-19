#ifndef HAVE_NODE_DEFINITIONS_H__
#define HAVE_NODE_DEFINITIONS_H__

/* 
Embed one of these macros into a structure to make it into a singly or doubly
linked list node, type argument must be the typename of the structure.
*/
#define SINGLY_ND(type) type *right
#define DOUBLY_ND(type) type *right, *left

#define def_singly_node(name, ...)											\
typedef struct name name;													\
struct name																	\
{																			\
	struct name *right;														\
	__VA_ARGS__																\
}																			\

#define def_doubly_node(name, ...)											\
typedef struct name name;													\
struct name																	\
{																			\
	struct name *right, *left;												\
	__VA_ARGS__																\
}																			\


#define db_del(pop)															\
{																			\
	(pop)->left->right = (pop)->right;										\
	(pop)->right->left = (pop)->left;										\
}																			\

/*
* Cases for non dummy lists:
* d_ptr->right to add next to (at tail is a case)
* *d_ptr to use double pointer technique
* d_ptr to add at head
* behaviour is similar for dummied lists, check add operations in
* stack.h, queue.h, :
* In those cases it inserts always after something:
* After dummy node for stacks
* After last inserted node in queues, as the add macro updates the tail
*/
#define sg_add(d_ptr, ins)													\
{																			\
	(ins)->right = (d_ptr);													\
	(d_ptr) = ins;															\
}																			\

// Link node after pos
#define db_link_after(pos, node)											\
{																			\
	node->right = (pos)->right;												\
	node->left = (pos);														\
}																			\

// Link node before pos
#define db_link_befor(pos, node)											\
{																			\
	node->right = (pos);													\
	node->left = (pos)->left;												\
}																			\

// Link left and right nodes to node
// [rightnode]-> [node] <-[leftnode]
#define db_link_left_and_right(node)										\
{																			\
	node->right->left = node,												\
	node->left->right = node;												\
}																			\


#define sg_dummy(dummy)														\
{																			\
	(dummy)->right = dummy;													\
}																			\

#define db_dummy(dummy)														\
{																			\
	dummy->right = dummy;													\
	dummy->left = dummy;													\
}																			\

// Recomended to use with double pointers
#define get_next(iter_ptr) ((iter_ptr)->right)

#define get_prev(iter_ptr) ((iter_ptr)->left)

// Recomended to use with simple pointers
#define mv_next(iter_ptr) ((iter_ptr) = (iter_ptr)->right)

#define mv_prev(iter_ptr) ((iter_ptr) = (iter_ptr)->left)

#endif // HAVE_NODE_DEFINITIONS_H__
