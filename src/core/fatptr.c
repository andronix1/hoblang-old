#include "fatptr.h"

FatPtr fatptr_empty() {
    FatPtr result = {
        .size = 0,
        .ptr = NULL
    };
    return result;
}

const char *fatptr_to_cstr(FatPtr *fatptr) {
	if (fatptr->str[fatptr->size - 1] == '\0') {
		return fatptr->str;
	}
	FatPtr other = fatptr_empty();
	fatptr_append(&other, fatptr);
	char a = 0;
	FatPtr c = {
		.size = 1,
		.ptr = &a
	};
	fatptr_append(&other, &c);
	return other.str;
}

FatPtr fatptr_from_cstr(const char *cstr) {
    FatPtr result = {
        .size = strlen(cstr),
        .ptr = (char*)cstr
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

bool fatptr_eq(FatPtr *ptr, FatPtr *other) {
	if (ptr->size != other->size) return false;
	return !memcmp(ptr->ptr, other->ptr, ptr->size);
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
	if (!fatptr->ptr) {
		hob_log(LOGE, "failed to allocate memory to read");
		return false;
	}
	if (fread(fatptr->ptr, 1, size, file) == (size_t)-1) {
		free(fatptr->ptr);
		hob_log(LOGE, "failed to read from file: %s\n", strerror(errno));
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

int printf_arginfo_fatptr(const struct printf_info *info __attribute__((unused)), size_t n, int *argtypes, int *size) {
	if (n > 0) {
		*argtypes = PA_POINTER;
		*size = sizeof(FatPtr*);
	}
	return 1;
}

int printf_output_fatptr(FILE *stream, const struct printf_info *info __attribute__((unused)), const void *const *args) {	
	return fatptr_print_to(*(FatPtr**)*args, stream);
}

void fatptr_register_printf() {
	register_printf_specifier('P', printf_output_fatptr, printf_arginfo_fatptr);
}
