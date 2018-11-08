#ifndef ITER_H
#define ITER_H

#include "types.h"

typedef ptr_t iter_t;

typedef struct _iterFunction {
	ptr_t (*get)(iter_t iter);
	void (*set)(iter_t iter, ptr_t data);
	iter_t (*step_next)(iter_t iter);
	iter_t (*step_prev)(iter_t iter);
	iter_t (*step_offset)(iter_t iter, long offset);
	ptrdiff_t (*diff)(iter_t iter1, iter_t iter2);
	iter_t (*copy)(iter_t iter);
	void (*free)(iter_t iter);
} iterFunction;

#endif

