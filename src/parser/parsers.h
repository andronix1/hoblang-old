#pragma once

#include "ast.h"
#include "parser.h"
#include "tokens.h"
#include "core/assert.h"

bool parse_module(Parser *parser, AstModule *module);
bool parse_module_node(Parser *parser, AstModuleNode *node);
bool parse_func_info(Parser *parser, AstFuncInfo *info);
bool parse_body(Parser *parser, AstBody *body);
bool parse_func_call_args(Parser *parser, AstFuncCall *func_call);
AstExpr *parse_expr_before(Parser *parser, bool (*stop)(TokenType));
bool parse_expr(Parser *parser, AstExpr *expr, bool (*stop)(TokenType));
bool parse_type(Parser *parser, AstType *type);
