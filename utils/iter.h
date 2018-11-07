#ifndef ITER_H
#define ITER_H

typedef struct _iterFunction {
	/* Input Iterator */
	ptrdiff_t (*getData)(void* iter);
	/* Output Iterator */
	void (*setData)(void* iter);
	/* Forward Iterator */
	void (*stepNext)(void* iter);
	/* Binary Iterator */
	void (*stepPrev)(void* iter);
	/* RandomAccess Iterator */
	void (*stepOffset)(void* iter, long offset);
	ptrdiff_t (*diff)(void* iter1, void* iter2);
} iterFunction;

#endif

