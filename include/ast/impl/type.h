#pragma once

#include "ast/api/expr.h"
#include "ast/api/type.h"
#include "ast/private/path.h"

AstType *ast_type_new_path(AstPath *path); 
AstType *ast_type_new_self(); 
AstType *ast_type_new_func(AstType **args, AstType *returns); 
AstType *ast_type_new_slice(AstType *of); 
AstType *ast_type_new_optional(AstType *of); 
AstType *ast_type_new_pointer(AstType *to); 
AstType *ast_type_new_array(AstType *of, AstExpr *length); 
