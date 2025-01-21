#pragma once

typedef struct {
	Slice *segments;
	union {
		struct _SemaValueDecl *value;
		struct _SemaTypeDecl *type;
		struct _SemaModule *module;
	};
} AstModPath;
