#include "fatptr.h"

FatPtr fatptr_empty() {
    FatPtr result = {
        .size = 0,
        .raw_ptr = NULL
    };
    return result;
}

void fatptr_alloc_more(FatPtr *fatptr, size_t size) {
    if (fatptr->raw_ptr == NULL) {
        fatptr->raw_ptr = malloc(size);
    } else {
        fatptr->raw_ptr = realloc(fatptr->raw_ptr, fatptr->size + size);
    }
    fatptr->size += size;
}

void fatptr_free(FatPtr *fatptr) {
    if (fatptr->raw_ptr != NULL) {
        free(fatptr->raw_ptr);
    }
}
