#pragma once

#include "parser/ast.h"
#include "value.h"
#include "module.h"
#include "project.h"

void sema_ast_body(SemaModule *sema, AstBody *body);
SemaType *sema_ast_type(SemaModule *sema, AstType *type);
SemaType *sema_ast_call(SemaModule *sema, AstCall *call);

SemaType *sema_resolve_value_path(SemaModule *sema, AstPath *path);
SemaType *sema_resolve_type_path(SemaModule *sema, AstPath *path);

bool sema_resolve_inner_value_path(SemaModule *sema, SemaType *type, AstInnerPath *path, size_t segment_idx, SemaValue *value);
bool sema_resolve_path(SemaModule *sema, AstPath *path, SemaValue *value);
SemaType *sema_value_expr_type(SemaModule *sema, AstExpr *expr, SemaType *expectation);
SemaType *sema_var_expr_type(SemaModule *sema, AstExpr *expr, SemaType *expectation);
