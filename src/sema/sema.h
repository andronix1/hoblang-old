#pragma once

#include "core/slice.h"
#include "type.h"

typedef struct {
	SemaType type;
	Slice name;
} SemaRecord;

typedef struct {
	SemaRecord *decls;
	SemaRecord *types;
} SemaScope;

typedef struct {
	SemaScope *scopes;
	bool failed;
} Sema;

#define sema_err(fmt, ...) \
	do { \
		hob_log(LOGE, fmt, ##__VA_ARGS__); \
		sema->failed = true; \
	} while (0)

Sema sema_new();
void sema_push_scope(Sema *sema);
SemaScope *sema_pop_scope(Sema *sema);
SemaType *sema_resolve_ident_type(Sema *sema, Slice *name);
void sema_push_type(Sema *sema, SemaRecord *type);
void sema_push_decl(Sema *sema, SemaRecord *type);
void sema_push_primitives(Sema *sema);
SemaType *sema_resolve_decl_type(Sema *sema, Slice *name);
