#pragma once

typedef enum {
    SEMA_VALUE_CONST = 1 << 0,
    SEMA_VALUE_VAR = 1 << 1,
    SEMA_VALUE_TYPE = 1 << 2,
    SEMA_VALUE_MODULE = 1 << 3
} SemaValueType;

typedef struct {
    SemaValueType type;
    struct _SemaType *sema_type;
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

static inline bool sema_value_const(SemaValue *value, struct _SemaType *type) {
    value->type = SEMA_VALUE_CONST;
    value->sema_type = type;
    return true;
}

static inline bool sema_value_type(SemaValue *value, struct _SemaType *type) {
    value->type = SEMA_VALUE_TYPE;
    value->sema_type = type;
    return true;
}
static inline bool sema_value_var(SemaValue *value, struct _SemaType *type) {
    value->type = SEMA_VALUE_VAR;
    value->sema_type = type;
    return true;
}
