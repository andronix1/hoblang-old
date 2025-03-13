#pragma once

#include <llvm-c/Core.h>
#include <string.h>
#include <malloc.h>
#include "sema/module/behaviour/impl.h"
#include "sema/type.h"
#include "sema/module.h"
#include "sema/const/const.h"

typedef enum {
    SEMA_VALUE_CONST,
    SEMA_VALUE_FINAL,
    SEMA_VALUE_VAR,
    SEMA_VALUE_TYPE,
    SEMA_VALUE_EXT_FUNC_HANDLE,
    SEMA_VALUE_BEHAVIOUR,
    SEMA_VALUE_MODULE
} SemaValueType;

typedef struct SemaValue {
    SemaValueType type;
    union {
        SemaConst constant;
        SemaType *sema_type;
        SemaModule *module;
        SemaBehaviour *behaviour;
    };
	LLVMValueRef ext_func_handle;
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

static inline SemaValue *sema_value_const(SemaConst constant) {
    SemaValue *result = malloc(sizeof(SemaValue));
	result->type = SEMA_VALUE_CONST;
	result->constant = constant;
	return result;
}

static inline SemaValue *sema_value_behaviour(SemaBehaviour *behaviour) {
    SemaValue *result = malloc(sizeof(SemaValue));
	result->type = SEMA_VALUE_BEHAVIOUR;
	result->behaviour = behaviour;
	return result;
}

SEMA_VALUE_CONSTRUCTOR(sema_value_ext_func_handle, SEMA_VALUE_EXT_FUNC_HANDLE)
SEMA_VALUE_CONSTRUCTOR(sema_value_final, SEMA_VALUE_FINAL)
SEMA_VALUE_CONSTRUCTOR(sema_value_type, SEMA_VALUE_TYPE)
SEMA_VALUE_CONSTRUCTOR(sema_value_var, SEMA_VALUE_VAR)

static inline SemaValue *sema_value_with_type(SemaValue *value, SemaType *type) {
	SemaValue *result = malloc(sizeof(SemaValue));
	memcpy(result, value, sizeof(SemaValue));
	result->sema_type = type;
	return result;
}
