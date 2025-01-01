#include "fatptr.h"

FatPtr fatptr_empty() {
    FatPtr result = {
        .size = 0,
        .ptr = NULL
    };
    return result;
}

size_t fatptr_print_to(const FatPtr *fatptr, FILE *stream) {
	return fwrite(fatptr->ptr, 1, fatptr->size, stream);
}

void fatptr_append(FatPtr *ptr, FatPtr *other) {
	size_t prev_size = ptr->size;
	fatptr_alloc_more(ptr, other->size);
	memcpy(ptr->str + prev_size, other->str, other->size);
}

void fatptr_alloc_more(FatPtr *fatptr, size_t size) {
    if (fatptr->ptr == NULL) {
        fatptr->ptr = malloc(size);
    } else {
        fatptr->ptr = realloc(fatptr->ptr, fatptr->size + size);
    }
    fatptr->size += size;
}

bool fatptr_read_all(FatPtr *fatptr, FILE *file) {
	fseek(file, 0, SEEK_END);
	size_t size = ftell(file);
	fseek(file, 0, SEEK_SET);
	fatptr->size = size;
	fatptr->ptr = malloc(size);
	if (fread(fatptr->ptr, 1, size, file) == (size_t)-1) {
		free(fatptr->ptr);
		printf("failed to read from file: %s\n", strerror(errno));
		return false;
	}
	return true;
}

void fatptr_print(FatPtr *fatptr) {
	for (size_t i = 0; i < fatptr->size; i++) {
		putchar(fatptr->str[i]);
	}
}

void fatptr_free(FatPtr *fatptr) {
    if (fatptr->ptr != NULL) {
        free(fatptr->ptr);
    }
}
