#pragma once

typedef struct {
	Slice *segments;
	union {
		struct _SemaValueDecl *value;
		struct _SemaModule *module;
	};
} AstModPath;
