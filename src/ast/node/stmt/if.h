#pragma once

#include <malloc.h>
#include "ast/interface/body.h"
#include "ast/interface/expr.h"

typedef struct {
    AstExpr *cond;
    AstBody *body;
} AstIfCondBlock;

typedef struct {
    AstIfCondBlock *if_block;
    AstIfCondBlock **else_ifs;
    AstBody *else_body;
} AstIf;

static inline AstIfCondBlock *ast_if_cond_block_new(AstExpr *cond, AstBody *body) {
    AstIfCondBlock *result = malloc(sizeof(AstIfCondBlock));
    result->cond = cond;
    result->body = body;
    return result;
}

static inline AstIf *ast_if_new(AstIfCondBlock *if_block, AstIfCondBlock **else_ifs, AstBody *else_body) {
    AstIf *result = malloc(sizeof(AstIf));
    result->if_block = if_block;
    result->else_ifs = else_ifs;
    result->else_body = else_body;
    return result;
}
