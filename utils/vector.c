#include <stdlib.h>
#include <assert.h>
#include "vector.h"

struct _vector {
	ptr_t* data;
	size_t len, capacity;
};



vector* vector_create() {
	vector* v = (vector*)malloc(sizeof(vector));
	v->data = NULL;
	v->len = 0;
	v->capacity = 0;
	return v;
}

vector* vector_create_init(size_t n) {
	vector* v = (vector*)malloc(sizeof(vector));
	v->data = (ptr_t*)calloc(n, sizeof(ptr_t));
	v->len = 0;
	v->capacity = n;
	return v;
}

vector* vector_copy(vector* v) {
	vector* newVector = vector_create_init(v->len);
	size_t i;
	for (i = 0; i < v->len; ++i) {
		newVector->data[i] = v->data[i];
	}
	return newVector;
}

vector* vector_copy_functional(vector* v, ptr_t (*copy)(ptr_t data)) {
	vector* newVector = vector_create_init(v->len);
	size_t i;
	for (i = 0; i < v->len; ++i) {
		newVector->data[i] = copy(v->data[i]);
	}
	return newVector;
}

void vector_destroy(vector *v) {
	free(v->data);
	free(v);
}

void vector_clear(vector *v) {
	free(v->data);
	v->data = NULL;
	v->len = 0;
	v->capacity = 0;
}

size_t vector_size(vector* v) {
	return v->len;
}

size_t vector_capacity(vector* v) {
	return v->capacity;
}

ptr_t vector_at(vector* v, size_t pos) {
	return v->data[pos];
}

void vector_push_back(vector *v, ptr_t data) {
	if (v->len == v->capacity) {
		v->capacity *= 2;
		v->data = realloc(v->data, sizeof(ptr_t) * v->capacity);
	}
	v->data[v->len++] = data;
}

void vector_pop_back(vector* v) {
	--v->len;
}

ptr_t vector_front(vector* v) {
	return v->data[0];
}

ptr_t vector_back(vector* v) {
	return v->data[v->len - 1];
}

void vector_shrink_to_fit(vector* v) {
	if (v->len) {
		v->data = realloc(v->data, sizeof(ptr_t) * v->len);
		v->capacity = v->len;
	} else {
		vector_clear(v);
	}
}

void vector_free_content(vector* v) {
	size_t i;
	for (i = 0; i < v->len; ++i) {
		free(v->data[i]);
	}
}

typedef struct _vectorIter {
	vector* v;
	size_t pos;
} vectorIter;

iter_t vector_begin(vector *v) {
	vectorIter* vi = (vectorIter*)malloc(sizeof(vectorIter));
	vi->v = v;
	vi->pos = 0;
	return vi;
}

iter_t vector_end(vector *v) {
	vectorIter* vi = (vectorIter*)malloc(sizeof(vectorIter));
	vi->v = v;
	vi->pos = v->len;
	return vi;
}

static ptr_t vector_iter_get(iter_t iter) {
	return ((vectorIter*)iter)->v->data[((vectorIter*)iter)->pos];
}

static void vector_iter_set(iter_t iter, ptr_t data) {
	((vectorIter*)iter)->v->data[((vectorIter*)iter)->pos] = data;
}

static iter_t vector_iter_step_next(iter_t iter) {
	++(((vectorIter*)iter)->pos);
	return iter;
}

static iter_t vector_iter_step_prev(iter_t iter) {
	--(((vectorIter*)iter)->pos);
	return iter;
}

static iter_t vector_iter_step_offset(iter_t iter, long offset) {
	((vectorIter*)iter)->pos += offset;
	return iter;
}

static ptrdiff_t vector_iter_diff(iter_t iter1, iter_t iter2) {
	return ((vectorIter*)iter2)->pos - ((vectorIter*)iter1)->pos;
}

static iter_t vector_iter_copy(iter_t iter) {
	vectorIter* vi = (vectorIter*)malloc(sizeof(vectorIter));
	vectorIter* p = (vectorIter*)iter;
	vi->v = p->v;
	vi->pos = p->pos;
	return vi;
}

static const iterFunction vectorIterFunction = {
	vector_iter_get,
	vector_iter_set,
	vector_iter_step_next,
	vector_iter_step_prev,
	vector_iter_step_offset,
	vector_iter_diff,
	vector_iter_copy,
	free
};

const iterFunction* vector_iter_function() {
	return &vectorIterFunction;
}
