#pragma once

#include "parser/ast.h"
#include "sema.h"

void sema_ast_body(SemaModule *sema, AstBody *body);
SemaType *sema_ast_type(SemaModule *sema, AstType *type);
SemaType *sema_ast_expr_type(SemaModule *sema, AstExpr *expr, SemaType *expectation);
SemaType *sema_ast_func_call(SemaModule *sema, AstFuncCall *func_call);
SemaType *sema_ast_value(SemaModule *sema, AstValue *value);
SemaTypeDecl *sema_resolve_type_path(SemaModule *sema, AstModPath *path);
SemaValueDecl *sema_resolve_value_path(SemaModule *sema, AstModPath *path);
SemaModule *sema_resolve_module_path(SemaModule *sema, AstModPath *path);
