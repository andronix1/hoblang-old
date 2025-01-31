#pragma once

typedef struct {
	struct _AstExpr *callable;
	struct _AstExpr **args;
} AstCall;
