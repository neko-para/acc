#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

typedef struct _vector vector;

vector* vector_create();
vector* vector_create_init(size_t n);
void vector_destroy(vector* v);
ptrdiff_t vector_at(vector* v, size_t pos);
void vector_push_back(vector* v, ptrdiff_t data);
void vector_pop_back(vector* v);
ptrdiff_t vector_front(vector* v);
ptrdiff_t vector_back(vector* v);
void vector_shrink_to_fit(vector* v);


#endif

