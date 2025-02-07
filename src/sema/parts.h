#pragma once

#include "parser/ast.h"
#include "parts/expr.h"
#include "value.h"
#include "module.h"
#include "project.h"

void sema_ast_body(SemaModule *sema, AstBody *body);
SemaType *sema_ast_type(SemaModule *sema, AstType *type);
SemaType *sema_ast_call(SemaModule *sema, AstCall *call);

SemaType *sema_resolve_value_path(SemaModule *sema, AstPath *path);
SemaType *sema_resolve_type_path(SemaModule *sema, AstPath *path);
SemaScopeDecl *sema_resolve_decl_path(SemaModule *sema, AstDeclPath *path);

SemaValue *sema_resolve_inner_value_path(SemaModule *sema, SemaType *type, AstInnerPath *path, size_t segment_idx);
SemaValue *sema_resolve_path(SemaModule *sema, AstPath *path);

SemaType *sema_value_expr_type(SemaModule *sema, AstExpr *expr, SemaExprCtx ctx);
SemaType *sema_var_expr_type(SemaModule *sema, AstExpr *expr, SemaExprCtx ctx);
SemaType *sema_const_expr_type(SemaModule *sema, AstExpr *expr, SemaExprCtx ctx);
uint64_t sema_eval_int_expr(SemaModule *sema, AstExpr *expr);