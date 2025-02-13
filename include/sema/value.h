#pragma once

#include <string.h>
#include <malloc.h>
#include "sema/type/type.h"

typedef enum {
    SEMA_VALUE_CONST = 1 << 0,
    SEMA_VALUE_VAR = 1 << 1,
    SEMA_VALUE_TYPE = 1 << 2,
    SEMA_VALUE_MODULE = 1 << 3
} SemaValueType;

typedef struct {
    SemaValueType type;
    SemaType *sema_type;
} SemaValue;

#define SEMA_VALUE_CONSTRUCTOR(name, value_type) \
	static inline SemaValue *name(SemaType *type) { \
		SemaValue *result = malloc(sizeof(SemaValue)); \
		result->type = value_type; \
		result->sema_type = type; \
		return result; \
	}

SEMA_VALUE_CONSTRUCTOR(sema_value_const, SEMA_VALUE_CONST);
SEMA_VALUE_CONSTRUCTOR(sema_value_type, SEMA_VALUE_TYPE);
SEMA_VALUE_CONSTRUCTOR(sema_value_var, SEMA_VALUE_VAR);

static inline SemaValue *sema_value_with_type(SemaValue *value, SemaType *type) {
	SemaValue *result = malloc(sizeof(SemaValue));
	memcpy(result, value, sizeof(SemaValue));
	result->sema_type = type;
	return result;
}
// static inline bool sema_value_const(SemaValue *value, struct _SemaType *type) {
//     value->type = ;
//     value->sema_type = type;
//     return true;
// }

// static inline bool sema_value_type(SemaValue *value, struct _SemaType *type) {
//     value->type = SEMA_VALUE_TYPE;
//     value->sema_type = type;
//     return true;
// }
// static inline bool sema_value_var(SemaValue *value, struct _SemaType *type) {
//     value->type = SEMA_VALUE_VAR;
//     value->sema_type = type;
//     return true;
// }
