#pragma once

#include "ast.h"
#include "private.h"
#include "tokens.h"
#include "core/assert.h"

bool parse_module(Parser *parser, AstModule *module);
bool parse_module_node(Parser *parser, AstModuleNode *node);
bool parse_func_info(Parser *parser, AstFuncInfo *info);
bool parse_body(Parser *parser, AstBody *body);
bool parse_body_ola(Parser *parser, AstBody *body);
AstExpr **parse_call_args(Parser *parser);
AstExpr *parse_expr(Parser *parser, bool (*stop)(TokenType));
bool parse_type(Parser *parser, AstType *type);
bool parse_path(Parser *parser, AstPath *path);
bool parse_decl_path(Parser *parser, AstDeclPath *path);
bool parse_inner_path(Parser *parser, AstInnerPath *path);