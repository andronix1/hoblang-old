#pragma once

#include "core/fatptr.h"
#include "type.h"

typedef struct {
	Type type;
	FatPtr name;
} Decl;

typedef Vec Decls;

typedef struct {
	Type type;
	FatPtr name;
} ScopeType;

typedef Vec ScopeTypes;

typedef struct {
	Decls decls;
	ScopeTypes types;
} Scope;

Scope scope_new();

typedef Vec ScopeStack;

typedef struct {
	ScopeStack scopes;
	bool failed;
} Sema;

Sema sema_new();
