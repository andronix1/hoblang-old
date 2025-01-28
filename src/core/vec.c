#include "vec.h"

VecHeader *vec_header(void *vec) {
	return &((VecHeader*)vec)[-1];
}

void *vec_new_sized(size_t esize) {
	VecHeader *header = malloc(sizeof(VecHeader));
	header->esize = esize;
	header->len = 0;
	header->cap = 0;
	return &header[1];
}

void *vec_reserve(void *vec, size_t cap) {
	VecHeader *header = vec_header(vec);
	if (header->cap >= cap) {
		return vec;
	}
	header = realloc(header, sizeof(VecHeader) + cap * header->esize);
	header->cap = cap;
	return &header[1];
}

void *vec_top(void *vec) {
	VecHeader *header = vec_header(vec);
	return (char*)vec + header->esize * (header->len - 1);
}

void *vec_pop(void *vec) {	
	VecHeader *header = vec_header(vec);
	assert(header->len > 0, "trying to pop empty vec");
	void *result = vec_top(vec);
	header->len--;
	return result;
}

void *vec_push(void *vec, void *element) {
	VecHeader *header = vec_header(vec);
	if (header->cap <= header->len) {
		vec = vec_reserve(vec, header->cap == 0 ? 1 : header->cap * 2);
		header = vec_header(vec);
	}
	memcpy((char*)vec + header->esize * header->len, element, header->esize);
	header->len++;
	return vec;
}

void *_vec_append_raw(void *vec, const void *ptr, size_t len) {
	VecHeader *header = vec_header(vec);
	Slice s = {
		.len = len,
		.str = ptr
	};
	print("adding {slice}\n", &s);
	vec = _vec_reserve(vec, header->len + len);
	for (size_t i = 0; i < len; i++) {
		vec = _vec_push(vec, (char*)ptr + i * header->esize);
	}
	return vec;
}

size_t vec_len(void *vec) {
	return vec_header(vec)->len;
}

void vec_free(void *vec) {
	free(vec_header(vec));
}
