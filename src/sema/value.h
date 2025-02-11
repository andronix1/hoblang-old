#pragma once

#include "sema/type/api.h"

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

/*
// test.hob
type Test = enum {
	Super,
	Duper,
	Cool
};
// TODO: think about syntax
fun print(self: Test, prefix: []u8) -> void {
	io::print(prefix);
	if self == Test.Super {
		io::println("super!");
	} else if test == Test.Duper {
		io::println("dooooper!");
	} else if test == Test.Cool {
		io::println("COOOOOOOOL!!!!");
	} else {
		io::println("UNKNOWNWNWNOWNOWNOWNo");
	}
}

// main.hob
use test;

fun main() -> void {
	test::Test.Cool.print("this is so ")	
	| module
	      | type
		       | const
			        | ext_func + const
}
*/

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
