#include "node.h"
#include "ast/interface/node.h"
#include "ast/node.h"
#include "ast/node/stmt.h"
#include "ast/node/stmt/assign.h"
#include "core/not_null.h"
#include "lexer/token/kind.h"
#include "parser/ast/decl/behaviour.h"
#include "parser/ast/decl/from_use.h"
#include "parser/ast/decl/func.h"
#include "parser/ast/decl/global.h"
#include "parser/ast/decl/import.h"
#include "parser/ast/decl/struct.h"
#include "parser/ast/decl/type.h"
#include "parser/ast/decl/use.h"
#include "parser/ast/decl/val.h"
#include "parser/ast/shared/expr.h"
#include "parser/ast/shared/global.h"
#include "parser/ast/stmt/asm.h"
#include "parser/ast/stmt/defer.h"
#include "parser/ast/stmt/loop_control.h"
#include "parser/ast/stmt/return.h"
#include "parser/ast/stmt/while.h"
#include "stmt/if.h"
#include "parser/api.h"

AstNodeStmt *parse_stmt_node_assign_binop(Parser *parser, AstExpr *expr, AstBinopKind kind) {
    AstExpr *value = NOT_NULL(parse_expr(parser)); 
    PARSER_EXPECT_NEXT(TOKEN_SEMICOLON, "`;`");
    return ast_node_stmt_new_assign(ast_assign_new_binop(expr, value, kind));
}

AstNodeStmt *parse_stmt_node(Parser *parser) {
    Token *token = parser_next(parser);    
    switch (token->kind) {
        case TOKEN_IF: return ast_node_stmt_new_if(NOT_NULL(parse_if(parser)));
        case TOKEN_ASM: return ast_node_stmt_new_inline_asm(NOT_NULL(parse_inline_asm(parser)));
        case TOKEN_DEFER: return ast_node_stmt_new_defer(NOT_NULL(parse_defer(parser)));
        case TOKEN_CONTINUE: return ast_node_stmt_new_continue(NOT_NULL(parse_loop_control(parser)));
        case TOKEN_BREAK: return ast_node_stmt_new_break(NOT_NULL(parse_loop_control(parser)));
        case TOKEN_RETURN: return ast_node_stmt_new_return(NOT_NULL(parse_return(parser)));
        case TOKEN_WHILE: return ast_node_stmt_new_while(NOT_NULL(parse_while(parser)));
        default: {
            parser_skip_next(parser);
            AstExpr *expr = NOT_NULL(parse_expr(parser));
            switch (parser_next(parser)->kind) {
                case TOKEN_SEMICOLON: return ast_node_stmt_new_expr(expr);
                case TOKEN_ASSIGN: {
                    AstExpr *value = NOT_NULL(parse_expr(parser)); 
                    PARSER_EXPECT_NEXT(TOKEN_SEMICOLON, "`;`");
                    return ast_node_stmt_new_assign(ast_assign_new_direct(expr, value));
                }
                case TOKEN_ASSIGN_ADD: return parse_stmt_node_assign_binop(parser, expr, AST_BINOP_ADD);
                case TOKEN_ASSIGN_MOD: return parse_stmt_node_assign_binop(parser, expr, AST_BINOP_MOD);
                case TOKEN_ASSIGN_MINUS: return parse_stmt_node_assign_binop(parser, expr, AST_BINOP_SUB);
                case TOKEN_ASSIGN_MULTIPLY: return parse_stmt_node_assign_binop(parser, expr, AST_BINOP_MUL);
                case TOKEN_ASSIGN_DIVIDE: return parse_stmt_node_assign_binop(parser, expr, AST_BINOP_DIV);
                default:
                    PARSE_ERROR("end of expression");
                    return NULL;
            }
            return NULL;
        }
    }
}

static AstNodeDecl *parse_decl_node(Parser *parser) {
    bool public = parser_next_is(parser, TOKEN_PUBLIC);
    Token *token = parser_next(parser);
    AstNodeDecl *result = NULL;
    switch (token->kind) {
        case TOKEN_FUN: result = parse_func_decl(parser); break;
        case TOKEN_STRUCT: result = parse_struct_decl(parser); break;
        case TOKEN_TYPE: result = parse_type_alias(parser); break;
        case TOKEN_EXTERN: result = ast_node_decl_new_external(parse_global_decl_info(parser)); break;
        case TOKEN_GLOBAL: result = parse_global_decl(parser); break;
        case TOKEN_BEHAVIOUR: result = parse_behaviour_decl(parser); break;
        case TOKEN_IMPORT: result = parse_import(parser); break;
        case TOKEN_USE: result = parse_use(parser); break;
        case TOKEN_FROM: result = parse_from_use(parser, token->location); break;
        case TOKEN_FINAL: case TOKEN_VAR: case TOKEN_CONST:
            parser_skip_next(parser);
            result = parse_val_decl(parser);
            break;
        default:
            parser_skip_next(parser);
            return NULL;
    }
    if (result) {
        result->public = public;
    }
    return result;
}

AstNode *parse_node(Parser *parser) {
    FileLocation loc = parser_next(parser)->location;
    parser_skip_next(parser);
    AstNodeDecl *decl = parse_decl_node(parser);
    if (decl) return ast_node_new_decl(loc, decl);
    AstNodeStmt *stmt = parse_stmt_node(parser);
    if (stmt) return ast_node_new_stmt(loc, stmt);
    PARSE_ERROR(EXPECTED("node"));
    return NULL;
}

