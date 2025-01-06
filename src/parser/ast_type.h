#pragma once

#include "sema/type.h"

typedef enum {
	AST_TYPE_IDENT
} AstTypeKind;

typedef struct {
	AstTypeKind type;
	Type sema;
	union {
		FatPtr ident;
	};
} AstType;
