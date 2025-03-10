#include "ast/impl/type.h"
#include "ast/private/type.h"

#define FIELD(src, dst) result->src = dst;
#define CONSTR(_type, fields) { \
        AstType *result = malloc(sizeof(AstType)); \
        result->type = _type; \
        result->sema = NULL; \
        fields; \
        return result; \
    }

AstType *ast_type_new_path(AstPath *path) CONSTR(AST_TYPE_PATH, {
    FIELD(path, path);
});
AstType *ast_type_new_func(AstType **args, AstType *returns) CONSTR(AST_TYPE_FUNCTION, {
    FIELD(func.args, args);
    FIELD(func.returns, returns);
});

AstType *ast_type_new_slice(AstType *of) CONSTR(AST_TYPE_SLICE, {
    FIELD(slice_of, of);
});

AstType *ast_type_new_optional(AstType *of) CONSTR(AST_TYPE_OPTIONAL, {
    FIELD(optional_of, of);
});

AstType *ast_type_new_pointer(AstType *to) CONSTR(AST_TYPE_POINTER, {
    FIELD(ptr_to, to);
});

AstType *ast_type_new_array(AstType *of, AstExpr *length) CONSTR(AST_TYPE_ARRAY, {
    FIELD(array.length, length);
    FIELD(array.of, of);
});

