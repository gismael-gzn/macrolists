#ifndef STACK_H__
#define STACK_H__

#include "node_definitions.h"
#include "container.h"

/* Embed a stack container type into a struct's definition */
#define STACK_CONTAINER(node_typename) \
	MCONTAINER_STRUCT_PART;            \
	node_typename dummy;

/* Embed a simple stack type into a struct's definition */
#define STACK(node_typename) \
	node_typename dummy;

/* typedef and define a stack container */
#define def_stack_container(cont_typename, node_typename) \
	typedef struct cont_typename cont_typename;           \
	struct cont_typename                                  \
	{                                                     \
		STACK_CONTAINER(node_typename)                    \
	}

/* typedef and define a simple stack */
#define def_stack(cont_typename, node_typename) \
	typedef struct cont_typename cont_typename; \
	struct cont_typename                        \
	{                                           \
		STACK(node_typename)                    \
	}

/* Get first stack's first node */
#define s_top(stack) (get_head(stack))

/* Macros for stack containers (keep stack's length in-memory) */

/* Add node in top of a stack */
#define sc_add(stack, node)        \
	{                              \
		sg_add(s_top(stack), node) \
			len_incr(stack);       \
	}

/* Pop stack's first node */
#define sc_pop(stack)          \
	{                          \
		mv_next(s_top(stack)); \
		len_decr(stack);       \
	}

/* Pop stack's first node and save it */
#define sc_get_pop(stack, save) \
	{                           \
		save = s_top(stack);    \
		mv_next(s_top(stack));  \
		len_decr(stack);        \
	}

/* Compound initializer for stack containers */
#define sc_compound(cont_type, cont_ptr, ...)                        \
	(cont_type)                                                      \
	{                                                                \
		.MCONT_LENGTH_MEMB = 0, .dummy.right = &get_dummy(cont_ptr), \
		__VA_ARGS__,                                                 \
	}

/* Stack container default initializer */
#define sc_init(stack)              \
	{                               \
		get_len(stack) = 0;         \
		sg_dummy(&get_dummy(stack)) \
	}

/* Macros for simple stacks (do not keep stack's length in-memory) */

/* Add node in top of a stack */
#define s_add(stack, node)         \
	{                              \
		sg_add(s_top(stack), node) \
	}

/* Pop stack's first node */
#define s_pop(stack)           \
	{                          \
		mv_next(s_top(stack)); \
	}

/* Pop stack's first node and save it */
#define s_get_pop(stack, save) \
	{                          \
		save = s_top(stack);   \
		mv_next(s_top(stack)); \
	}

/* Compound initializer for simple stacks */
#define s_compound(cont_type, cont_ptr, ...) \
	(cont_type) { .dummy.right = &get_dummy(cont_ptr), __VA_ARGS__ }

/* Simple stack default initializer */
#define s_init(stack) sg_dummy(&get_dummy(stack))

#endif // STACK_H__
