#ifndef SINGLY_H__
#define SINGLY_H__

#include "node_definitions.h"
#include "container.h"

/* 
* It's advised to have some knowledge about double pointers (T**) to understand
* this file a bit better. Good thing is that this is the only linked list
* in this project implemented with such gimmicks, and the only reason being that
* this kind of list may have non-restricted insertions, unlike stacks or queues
* where it's fairly easy to keep track of the head and tail respectively.
* In the case of doubly linked lists, it's not needed, since one can easily get
* the previous node to correctly relink nodes after insertions/deletions.  
*/

/* Embed a list container type into a struct's definition */
#define SINGLY_LIST_CONTAINER(node_typename) \
	MCONTAINER_STRUCT_PART;                  \
	node_typename dummy;

/* Embed a simple list type into a struct's definition */
#define SINGLY_LIST(node_typename) \
	node_typename dummy;

/* typedef and define a list container */
#define def_singly_list_container(cont_typename, node_typename) \
	typedef struct cont_typename cont_typename;                 \
	struct cont_typename                                        \
	{                                                           \
		SINGLY_LIST_CONTAINER(node_typename)                    \
	}

/* typedef and define a simple list */
#define def_singly_list(cont_typename, node_typename) \
	typedef struct cont_typename cont_typename;       \
	struct cont_typename                              \
	{                                                 \
		SINGLY_LIST(node_typename)                    \
	}


/* Macros for list containers (keep list's length in-memory) */

/* Insert node to the right of what *pos node points to */
#define slc_add(list, pos, node) \
	{                            \
		sg_add(*pos, node)       \
			len_incr(list);      \
	}

/* Pop the node pointed to by *pos */
#define slc_pop(list, pos) \
	{                      \
		mv_next(*pos);     \
		len_decr(list);    \
	}

/* Pop the node pointed to by *pos and save it */
#define slc_get_pop(list, pos, save) \
	{                                \
		save = *pos;                 \
		mv_next(*pos);               \
		len_decr(list);              \
	}

/* Compound initializer for list containers */
#define slc_compound(cont_type, list, ...) \
	(cont_type){                           \
		.MCONT_LENGTH_MEMB = 0,            \
		.dummy.right = &get_dummy(list),   \
		__VA_ARGS__,                       \
	};

/* List container default initializer */
#define slc_init(list)              \
	{                               \
		get_len(list) = 0;          \
		sg_dummy(&get_dummy(list)); \
	}


/* Macros for simple lists (do not keep list's length in-memory) */

/* Insert node to the right of what *pos node points to */
#define sl_add(pos, node) sg_add(*pos, node)

/* Pop the node pointed to by *pos */
#define sl_pop(pos) mv_next(*pos);

/* Pop the node pointed to by *pos and save it */
#define sl_get_pop(pos, save) \
	{                                \
		save = *pos;                 \
		mv_next(*pos);               \
	}

/* Compound initializer for simple lists */
#define sl_compound(cont_type, list, ...) \
	(cont_type){.dummy.right = &get_dummy(list), __VA_ARGS__,};

/* Simple list default initializer */
#define sl_init(list) sg_dummy(&get_dummy(list));

#endif // SINGLY_H__
