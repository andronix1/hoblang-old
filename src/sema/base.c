#include "base.h"

Scope scope_new() {
	Scope result = {
		.decls = vec_new(Decl),
		.types = vec_new(ScopeType),
	};
	return result;
}

Sema sema_new() {
	Sema result = {
		.scopes = vec_new(Scope),
		.failed = false
	};
	return result;
}
