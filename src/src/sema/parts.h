#pragma once

#include "parser/ast.h"
#include "module.h"
#include "project.h"

void sema_ast_body(SemaModule *sema, AstBody *body);
SemaType *sema_ast_type(SemaModule *sema, AstType *type);
SemaType *sema_ast_expr_type(SemaModule *sema, AstExpr *expr, SemaType *expectation);
SemaType *sema_ast_func_call(SemaModule *sema, AstFuncCall *func_call);
SemaType *sema_ast_value(SemaModule *sema, AstValue *value);

SemaScopeValueDecl *sema_resolve_mod_path_value(SemaModule *sema, AstModPath *path);
SemaModule *sema_resolve_mod_path_module(SemaModule *sema, AstModPath *path);
SemaType *sema_resolve_mod_path_type(SemaModule *sema, AstModPath *path);