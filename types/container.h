#ifndef HAVE_CONTAINER_H__
#define HAVE_CONTAINER_H__

#include <stdlib.h>
#include <assert.h>

#define MCONT_LENGTH_MEMB mcont_length__
#define MCONTAINER_STRUCT_PART size_t MCONT_LENGTH_MEMB


// Increment and decrement the length variable of a container.
#define len_decr(container) (--(container)->MCONT_LENGTH_MEMB)

#define len_incr(container) (++(container)->MCONT_LENGTH_MEMB)


// Get the length member of a container
#define get_len(container) ((container)->MCONT_LENGTH_MEMB)

// Get the head of a linked list (doubly and singly)
#define get_head(list) ((list)->dummy.right)

// Get the tail of a linked list (doubly)
#define get_tail(list) ((list)->dummy.left)

// Iterator utilities:

#define get_dummy(container) ((container)->dummy)

// From a double pointer iterator, checks if the iterator has reached dummy node
#define iauto(container, iterator) ((iterator) != &(container)->dummy)

// Given a number "num", checks if num is smaller than the lenght of the container
#define nauto(container, num) ((num) < get_len(container))

#define cyclic_next(cont, iter, op)\
(iauto(cont, get_next(iter))? op get_next(iter): op get_head(cont))\

#define cyclic_prev(cont, iter, op)\
(iauto(cont, get_prev(iter))? op get_prev(iter): op get_tail(cont))\

// Check wether a container is empty
#define lempty(container) (get_head(container) == &(container)->dummy)

#endif // HAVE_CONTAINER_H__
