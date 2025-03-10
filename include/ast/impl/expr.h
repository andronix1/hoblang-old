#pragma once

#include <stdbool.h>
#include "ast/api/body.h"
#include "ast/api/expr.h"
#include "ast/api/func_info.h"
#include "ast/api/path.h"
#include "ast/api/type.h"
#include "ast/private/expr.h"
#include "ast/private/expr/binop.h"
#include "ast/private/expr/struct.h"
#include "core/location.h"

AstExpr *ast_expr_anon_fun(FileLocation loc, AstFuncTypeInfo info, AstBody body);
AstExpr *ast_expr_struct(FileLocation loc, AstPath *path, AstExprStructMember *members);
AstExpr *ast_expr_ret_on_null(FileLocation loc, AstExpr *expr);
AstExpr *ast_expr_unwrap(FileLocation loc, AstExpr *expr, Slice name);
AstExpr *ast_expr_get_local_path(FileLocation loc, AstPath *path);
AstExpr *ast_expr_get_inner_path(FileLocation loc, AstExpr *of, AstPath *path);
AstExpr *ast_expr_idx(FileLocation loc, AstExpr *of, AstExpr *idx);
AstExpr *ast_expr_integer(FileLocation loc, uint64_t value);
AstExpr *ast_expr_float(FileLocation loc, long double value);
AstExpr *ast_expr_char(FileLocation loc, char value);
AstExpr *ast_expr_str(FileLocation loc, Slice value);
AstExpr *ast_expr_bool(FileLocation loc, bool value);
AstExpr *ast_expr_call(FileLocation loc, AstExpr *callable, AstExpr **args);
AstExpr *ast_expr_not(FileLocation loc, AstExpr *expr);
AstExpr *ast_expr_as_type(FileLocation loc, FileLocation as_loc, AstExpr *expr, AstType *type);
AstExpr *ast_expr_as_auto(FileLocation loc, FileLocation as_loc, AstExpr *expr);
AstExpr *ast_expr_binop(FileLocation loc, AstBinopType type, AstExpr *left, AstExpr *right);
AstExpr *ast_expr_unary(FileLocation loc, AstUnaryType type, AstExpr *expr);
AstExpr *ast_expr_array(FileLocation loc, AstExpr **values);
AstExpr *ast_expr_ref(FileLocation loc, AstExpr *expr);
AstExpr *ast_expr_null(FileLocation loc);
