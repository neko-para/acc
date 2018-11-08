#ifndef VECTOR_H
#define VECTOR_H

#include "types.h"
#include "iter.h"

typedef struct _vector vector;

vector* vector_create();
vector* vector_create_init(size_t n);
vector* vector_copy(vector* v);
vector* vector_copy_functional(vector* v, ptr_t (*copy)(ptr_t data));
void vector_destroy(vector* v);
void vector_clear(vector* v);
size_t vector_size(vector* v);
size_t vector_capacity(vector* v);
ptr_t vector_at(vector* v, size_t pos);
void vector_push_back(vector* v, ptr_t data);
void vector_pop_back(vector* v);
ptr_t vector_front(vector* v);
ptr_t vector_back(vector* v);
void vector_shrink_to_fit(vector* v);
void vector_free_content(vector* v);
iter_t vector_begin(vector* v);
iter_t vector_end(vector* v);

const iterFunction* vector_iter_function();

#endif

