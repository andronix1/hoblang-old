#pragma once

#include "parser/ast.h"
#include "sema.h"

void sema_ast_module(Sema *sema, AstModule *module);
bool sema_ast_type(Sema *sema, SemaType *output, AstType *type);
void sema_ast_body(Sema *sema, AstBody *body, SemaType *returning);
bool sema_ast_expr_type(Sema *sema, SemaType *type, AstExpr *expr, SemaType*expectation);
bool sema_ast_func_call(Sema *sema, SemaType *output_type, AstFuncCall *func_call);
