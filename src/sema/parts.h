#pragma once

#include "parser/ast.h"
#include "module.h"
#include "project.h"

void sema_ast_body(SemaModule *sema, AstBody *body);
SemaType *sema_ast_type(SemaModule *sema, AstType *type);
SemaType *sema_ast_expr_type(SemaModule *sema, AstExpr *expr, SemaType *expectation);
SemaType *sema_ast_call(SemaModule *sema, AstCall *call);

SemaType *sema_resolve_path_type(SemaModule *sema, AstPath *path);
