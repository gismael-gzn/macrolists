#ifndef DOUBLY_H__
#define DOUBLY_H__

#include "node_definitions.h"
#include "container.h"

/* Embed a list container type into a struct's definition */
#define DOUBLY_LIST_CONTAINER(node_typename) \
	MCONTAINER_STRUCT_PART;                  \
	node_typename dummy;

/* Embed a simple list type into a struct's definition */
#define DOUBLY_LIST(node_typename) \
	node_typename dummy;

/* typedef and define a list container */
#define def_doubly_list_container(cont_typename, node_typename) \
	typedef struct cont_typename cont_typename;                 \
	struct cont_typename                                        \
	{                                                           \
		DOUBLY_LIST_CONTAINER(node_typename)                    \
	}

/* typedef and define a simple list */
#define def_doubly_list(cont_typename, node_typename) \
	typedef struct cont_typename cont_typename;       \
	struct cont_typename                              \
	{                                                 \
		DOUBLY_LIST(node_typename)                    \
	}

/* Macros for list containers (keep list's length in-memory) */

/* Insert node before pos */
#define dlc_insb(list, pos, node)        \
	{                                    \
		db_link_befor(pos, node)         \
			db_link_left_and_right(node) \
				len_incr(list);          \
	}

/* Insert node after pos */
#define dlc_insa(list, pos, node)        \
	{                                    \
		db_link_after(pos, node)         \
			db_link_left_and_right(node) \
				len_incr(list);          \
	}

/* Pop the node pointer to by node */
#define dlc_pop(list, node) \
	{                       \
		db_del(node)        \
			len_decr(list); \
	}

/* 
* There is no need to add ##get_pop macros here since the value of node doesn't
* get changed when poped (as in singly linked lists) and there's no potential
* way in which the pointer to the node can be lost (as in stacks/queues).
*/

/* Compound initializer for list containers */
#define dlc_compound(cont_typename, list, ...)                   \
	(cont_typename)                                              \
	{                                                            \
		.MCONT_LENGTH_MEMB = 0, .dummy.right = &get_dummy(list), \
		.dummy.left = &get_dummy(list), __VA_ARGS__,             \
	}

/* List container default initializer */
#define dlc_init(list)          \
	{                           \
		get_len(list) = 0;      \
	db_dummy((&get_dummy(list)) \
	}

/* Macros for simple lists (do not keep list's length in-memory) */

/* Insert node before pos */
#define dl_insb(pos, node)         \
	{                                    \
		db_link_befor(pos, node)         \
			db_link_left_and_right(node) \
	}

/* Insert node after pos */
#define dl_insa(pos, node)         \
	{                                    \
		db_link_after(pos, node)         \
			db_link_left_and_right(node) \
	}

/* Pop the node pointer to by node */
#define dl_pop(node) db_del(node)

#define dl_compound(cont_typename, list, ...)                            \
	(cont_typename)                                                      \
	{                                                                    \
		.dummy.right = &get_dummy(list), .dummy.left = &get_dummy(list), \
		__VA_ARGS__,                                                     \
	}

/* Simple list default initializer */
#define dl_init(list) db_dummy((&get_dummy(list))

#endif // SINGLY_H__
