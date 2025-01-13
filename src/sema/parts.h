#pragma once

#include "parser/ast.h"
#include "sema.h"

void sema_ast_module(Sema *sema, AstModule *module);
void sema_ast_body(Sema *sema, AstBody *body);
SemaType *sema_ast_type(Sema *sema, AstType *type);
SemaType *sema_ast_expr_type(Sema *sema, AstExpr *expr, SemaType *expectation);
SemaType *sema_ast_func_call(Sema *sema, AstFuncCall *func_call);
SemaType *sema_ast_value(Sema *sema, AstValue *value);
