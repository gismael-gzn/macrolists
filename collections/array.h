#ifndef ARRAY_H__
#define ARRAY_H__

#include <string.h>
#include "container.h"

#define ARRAY_INITIAL_SIZE 2UL

#if (ARRAY_INITIAL_SIZE == 0)
#	error "malloc(0) is implementation defined, define a different size"
#endif

/* 
* These macros expand to a function expected to at least have the behaviour
* of their stdlib default expansion.
* These hooks are fundamental for creation, insertion, deletion and concatenation
* of array containers, code-user can redefine them for debugging or other porpuses,
* for example make something like:
#define MALLOC_HOOK(n) my_malloc(n, __FUNCTION__, __LINE__), or the like.
*/

#ifndef MALLOC_HOOK
#	define MALLOC_HOOK malloc
#endif

#ifndef REALLOC_HOOK
#	define REALLOC_HOOK realloc
#endif

#ifndef FREE_HOOK
#	define FREE_HOOK free
#endif

#ifndef MEMMOVE_HOOK
#	define MEMMOVE_HOOK memmove
#endif

/* Macro to embed an array type into a structure definition */
#define ARRAY_CONTAINER(type)\
MCONTAINER_STRUCT_PART, slots;\
type* array;\

/* typedef and define a struct which contains an array of type 'type' represented
into the structure as a pointer to type */
#define def_array_container(cont_typename, type)\
typedef struct cont_typename cont_typename;\
struct cont_typename\
{\
	ARRAY_CONTAINER(type)\
}\

// Fundamental macros in this file
#define a_isz ARRAY_INITIAL_SIZE
#define a_slots(cont) ((cont)->slots)
#define slo_decr(cont) (--a_slots(cont))
#define slo_incr(cont) (++a_slots(cont))
#define arr(cont) (cont)->array
#define ini_allocsz(cont) (a_isz * sizeof*arr(cont))
#define siz_allocsz(cont, siz) ((siz) * sizeof*arr(cont))
#define dup_allocsz(cont) (2*(a_slots(cont)==0? a_isz: a_slots(cont))*sizeof*arr(cont))
#define shr_allocsz(cont) (get_len(cont)*sizeof*arr(cont))

//Avoid floating point exceptions, used in modulo operations
#define avoid_fpe(l) (l==0? 1: l)

// Get element n or index element n from array, 
// the macro ensures that n is within the range of the array's lenght
#define a_get(cont, n) (arr(cont)+( (n) % avoid_fpe(get_len(cont))))
#define a_idx(cont, n) (arr(cont)[ (n) % avoid_fpe(get_len(cont)) ])


/*
Given a pointer of type arr(cont), calculates the offset in number of elements 
from the array's base to ptr. That means, ptr must point to an address within
the range [array's base/initial address, initial address + array length]
*/
#define a_ptroffset(cont, ptr) ((size_t)(ptr - arr(cont)))

// Always initialize after allocation
#define a_init(cont)														\
{																			\
	get_len(cont) = 0;														\
	a_slots(cont) = a_isz,													\
	arr(cont) = MALLOC_HOOK(ini_allocsz(cont));								\
}

#define a_init_siz(cont, siz)												\
{																			\
	get_len(cont) = 0;														\
	a_slots(cont) = siz,													\
	arr(cont) = MALLOC_HOOK(siz_allocsz(cont, siz));						\
}

#define a_compound(cont_typename, cont, initial_size, ...)					\
(cont_typename){															\
	.MCONT_LENGTH_MEMB = 0, .slots = initial_size,							\
	.array = MALLOC_HOOK(siz_allocsz(cont, initial_size)),					\
	__VA_ARGS__															\
}																			\

// Reset an array to an empty state
#define a_del(cont)															\
{																			\
	FREE_HOOK(arr(cont)),													\
	get_len(cont) = a_slots(cont) = 0,										\
	arr(cont) = NULL;														\
}																			\


// Insert elem, which is always going to be inserted at i%(length+1)
#define a_ins(cont, i, elem)												\
{																			\
	size_t l=get_len(cont), m=(i)%(l+1);									\
	if(a_slots(cont) == 0)													\
		a_slots(cont) = get_len(cont),										\
		arr(cont) = REALLOC_HOOK(arr(cont), dup_allocsz(cont));				\
	MEMMOVE_HOOK(arr(cont)+m+1, arr(cont)+m, sizeof*arr(cont)*(l-m));		\
	arr(cont)[m] = elem, slo_decr(cont), len_incr(cont);					\
}																			\


// Insert at the end of the array
#define a_add(cont, elem)													\
{																			\
	if(a_slots(cont) == 0)													\
		a_slots(cont) = get_len(cont),										\
		arr(cont) = REALLOC_HOOK(arr(cont), dup_allocsz(cont));				\
	arr(cont)[get_len(cont)] = elem, slo_decr(cont), len_incr(cont);		\
}\

/*
Pops the element given an index, index gets modulated to be within the range
of the array's lenght
*/
#define a_pop(cont, i)														\
{																			\
	size_t l=get_len(cont), m=(i)%avoid_fpe(l);								\
	MEMMOVE_HOOK(arr(cont)+m, arr(cont)+m+1, sizeof*arr(cont)*(l-m)),		\
	slo_incr(cont), len_decr(cont);											\
	if(get_len(cont) == a_slots(cont))										\
		a_slots(cont) = 0,													\
		arr(cont) = REALLOC_HOOK(arr(cont), shr_allocsz(cont));				\
}\

// Grow the available slots in the array by new_elems
#define a_grow(cont, new_elems)												\
{																			\
	a_slots(cont) += (new_elems),											\
	arr(cont) = REALLOC_HOOK(arr(cont),										\
		sizeof*arr(cont)*(get_len(cont)+a_slots(cont)));					\
}																			\

// Shrink the array to size new_total of elements
// if new_total is 0 the array gets deleted, the new_total must be smaller than 
// lenght + slots, otherwise, no action is performed
#define a_shrn(cont, new_total)												\
{																			\
	if(new_total == 0)														\
		a_del(cont)															\
	else if(new_total < get_len(cont)+a_slots(cont))						\
		a_slots(cont) = 0,													\
		get_len(cont) = new_total,											\
		REALLOC_HOOK(arr(cont), sizeof*arr(cont)*(new_total));					\
}																			\

// Concatenate the contents of cont2 to cont1, it works even if cont1==cont2
#define a_ccat(cont1, cont2)												\
{																			\
	size_t l1=get_len(cont1), l2=get_len(cont2), s=a_slots(cont1);			\
	if(s < l2)																\
		a_slots(cont1) = 0,													\
		arr(cont1) = REALLOC_HOOK(arr(cont1), sizeof*arr(cont1)*(l1+l2));		\
	else																	\
		a_slots(cont1) -= l2;												\
	MEMMOVE_HOOK(arr(cont1)+l1, arr(cont2), sizeof*arr(cont2)*l2);				\
	get_len(cont1) += l2;													\
}																			\


// Concatenate n elements of contained size from mem to cont1
#define a_ccat_raw(cont1, mem, elements)									\
{																			\
	size_t l1=get_len(cont1), l2=elements, s=a_slots(cont1);				\
	if(s < l2)																\
		a_slots(cont1) = 0,													\
		arr(cont1) = REALLOC_HOOK(arr(cont1), sizeof*arr(cont1)*(l1+l2));		\
	else																	\
		a_slots(cont1) -= l2;												\
	MEMMOVE_HOOK(arr(cont1)+l1, mem, sizeof*arr(cont1)*l2);						\
	get_len(cont1) += l2;													\
}																			\

// Concatenate n elements of contained size from mem to cont1
// Using special memmove function 
#define a_ccat_raw_with(cont1, mem, elements, using_memmove)				\
{																			\
	size_t l1=get_len(cont1), l2=elements, s=a_slots(cont1);				\
	if(s < l2)																\
		a_slots(cont1) = 0,													\
		arr(cont1) = REALLOC_HOOK(arr(cont1), sizeof*arr(cont1)*(l1+l2));		\
	else																	\
		a_slots(cont1) -= l2;												\
	using_memmove(arr(cont1)+l1, mem, sizeof*arr(cont1)*l2);					\
	get_len(cont1) += l2;													\
}																			\

/*
Chop an interval of the array and move items located in arr+to and forward
to arr+from, from-to slots left available, the macro ensures that:
from<to and both are within the range of the container's lenght, caller can
chop the entire list which is equivalent to a_del, if the space gets shrinked 
by half, the array gets reallocated to half the original size
*/
#define a_chop(cont, from, to)												\
{																			\
	size_t l=avoid_fpe(get_len(cont)), from_=(from)%l, to_=(to)%(l+1),		\
	interval;																\
	if(from_ > to_)															\
	 	to_ ^= from_, from_ ^= to_, to_ ^= from_;							\
	interval = to_-from_;													\
	if(interval != 0)														\
	 	MEMMOVE_HOOK(arr(cont)+from_, arr(cont)+to_, sizeof*arr(cont)*(l-to_)),	\
	 	a_slots(cont) += interval,											\
	 	get_len(cont) -= interval;											\
	if(interval == l)														\
		a_del(cont)															\
	else if(get_len(cont) == a_slots(cont))									\
		a_slots(cont) = 0,													\
		arr(cont) = REALLOC_HOOK(arr(cont), shr_allocsz(cont));				\
}

//Conditional to be used in loops, given a pointer of type arr(cont)
#define a_auto(cont, i) ((i)<arr(cont)+get_len(cont))

/*
Notice that any realloc that changes the array pointer may left i pointing to
a non valid adress, it's adviced to use numeric control variables in cases of
insertions and deletions within the array
*/
#define array_traversal(cont, type, i)\
type i=a_get(cont, 0); a_auto(cont, i); ++i

#endif // ARRAY_H__
