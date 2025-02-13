#pragma once

#include <string.h>
#include <malloc.h>
#include "sema/type.h"
#include "sema/module.h"

typedef enum {
    SEMA_VALUE_CONST,
    SEMA_VALUE_VAR,
    SEMA_VALUE_TYPE,
    SEMA_VALUE_MODULE
} SemaValueType;

typedef struct SemaValue {
    SemaValueType type;
    union {
        SemaType *sema_type;
        SemaModule *module;
    };
    struct AstExpr *integer_expr; // TODO: REMOVE THIS!!!
} SemaValue;

#define SEMA_VALUE_CONSTRUCTOR(name, value_type) \
	static inline SemaValue *name(SemaType *type) { \
		SemaValue *result = malloc(sizeof(SemaValue)); \
		result->type = value_type; \
		result->sema_type = type; \
		return result; \
	}
	
static inline SemaValue *sema_value_module(SemaModule *module) {
    SemaValue *result = malloc(sizeof(SemaValue));
	result->type = SEMA_VALUE_MODULE;
	result->module = module;
	return result;
}

SEMA_VALUE_CONSTRUCTOR(sema_value_const, SEMA_VALUE_CONST)
SEMA_VALUE_CONSTRUCTOR(sema_value_type, SEMA_VALUE_TYPE)
SEMA_VALUE_CONSTRUCTOR(sema_value_var, SEMA_VALUE_VAR)

static inline SemaValue *sema_value_with_type(SemaValue *value, SemaType *type) {
	SemaValue *result = malloc(sizeof(SemaValue));
	memcpy(result, value, sizeof(SemaValue));
	result->sema_type = type;
	return result;
}
