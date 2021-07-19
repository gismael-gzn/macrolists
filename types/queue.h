#ifndef QUEUE_H__
#define QUEUE_H__

#include "node_definitions.h"
#include "container.h"

/* Embed a queue container type into a struct's definition */
#define QUEUE_CONTAINER(node_typename)\
MCONTAINER_STRUCT_PART;\
node_typename *tail, dummy;\

/* Embed a simple queue type into a struct's definition */
#define QUEUE(node_typename)\
node_typename *tail, dummy;\

/* typedef and define a queue container */
#define def_queue_container(cont_typename, node_typename)\
typedef struct cont_typename cont_typename;\
struct cont_typename\
{\
	QUEUE_CONTAINER(node_typename)\
}\

/* typedef and define a simple queue */
#define def_queue(cont_typename, node_typename)\
typedef struct cont_typename cont_typename;\
struct cont_typename\
{\
	QUEUE(node_typename)\
}\

// Get queue's first element
#define q_first(queue) get_head(queue)

// Get queue's last elemnt
#define q_last(queue) ((queue)->tail)


/* Macros for queue containers (keep queue's length in-memory) */

/* Add node at queue's head */
#define qc_add(queue, node)\
{\
	sg_add(q_last(queue)->right, node);\
	mv_next(q_last(queue));\
	len_incr(queue);\
}\

/* Pop queue's first node */
#define qc_pop(queue)\
{\
	mv_next(q_first(queue));\
	len_decr(queue);\
}\

/* Pop queue's first node and save it */
#define qc_get_pop(queue)\
{\
	save = q_first(queue);\
	mv_next(q_first(queue));\
	len_decr(queue);\
}\

/* Compound initializer for queue containers */
#define qc_compound(cont_typename, cont_ptr, ...)\
(cont_typename){\
	.MCONT_LENGTH_MEMB = 0, .tail = &get_dummy(cont_ptr),\
 	.dummy.right = &get_dummy(cont_ptr), __VA_ARGS__,\
}\

/* Queue container default initializer */
#define qc_init(queue)\
{\
	get_len(queue) = 0;\
	(queue)->tail = &(queue)->dummy;\
	sg_dummy((queue)->tail);\
}\


/* Macros for simple queues (do not keep queue's length in-memory) */

/* Add node at queue's head */
#define q_add(queue, node)\
{\
	sg_add(q_last(queue)->right, node);\
	mv_next(q_last(queue));\
}\

/* Pop queue's first node */
#define q_pop(queue)\
{\
	mv_next(q_first(queue));\
}\

/* Pop queue's first node and save it */
#define q_get_pop(queue, save)\
{\
	save = q_first(queue);\
	mv_next(q_first(queue));\
}\

/* Compound initializer for queue containers */
#define q_compound(cont_typename, cont_ptr, ...)\
(cont_typename){\
	.tail = &get_dummy(cont_ptr), .dummy.right = &get_dummy(cont_ptr),\
	__VA_ARGS__,\
}\

/* Queue container default initializer */
#define q_init(queue)\
{\
	(queue)->tail = &(queue)->dummy;\
	sg_dummy((queue)->tail);\
}\


#endif // QUEUE_H__
