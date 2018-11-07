#include <stdlib.h>
#include <assert.h>
#include "vector.h"

struct _vector {
	ptrdiff_t* data;
	size_t len, capacity;
};

vector* vector_create() {
	vector* v = (vector*)malloc(sizeof(vector));
	v->data = 0;
	v->len = 0;
	v->capacity = 0;
}

vector* vector_create_init(size_t n) {
	vector* v = (vector*)malloc(sizeof(vector));
	v->data = (ptrdiff_t*)calloc(n, sizeof(ptrdiff_t));
	v->len = 0;
	v->capacity = n;
}

void vector_destroy(vector *v) {
	free(v->data);
	free(v);
}

ptrdiff_t vector_at(vector* v, size_t pos) {
	return v->data[pos];
}

void vector_push_back(vector *v, ptrdiff_t data) {
	if (v->len == v->capacity) {
		v->capacity *= 2;
		v->data = realloc(v->data, sizeof(ptrdiff_t) * v->capacity);
	}
	v->data[v->len++] = data;
}

void vector_pop_back(vector* v) {
	--v->len;
}

ptrdiff_t vector_front(vector* v) {
	return v->data[0];
}

ptrdiff_t vector_back(vector* v) {
	return v->data[v->len - 1];
}

void vector_shrink_to_fit(vector* v) {
	v->data = realloc(v->data, sizeof(ptrdiff_t) * v->len);
	v->capacity = v->len;
}
