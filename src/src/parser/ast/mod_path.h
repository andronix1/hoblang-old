#pragma once

typedef struct {
	Slice *segments;
	union {
		struct _SemaScopeValueDecl *value;
		struct _SemaType *type;
		struct _SemaModule *module;
	};
} AstModPath;
