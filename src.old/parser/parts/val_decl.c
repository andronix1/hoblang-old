#include "parser/parts/val_decl.h"
#include "ast/private/val_decl.h"
#include "lexer/token.h"
#include "parser/parts/expr.h"
#include "parser/parts/type.h"
#include "parser/private.h"
#include "parser/token_stops.h"

bool parse_val_decl(Parser *parser, AstValDecl *decl) {
    Token *type_token = parser_next(parser);
    switch (type_token->type) {
        case TOKEN_VAR: decl->type = AST_VAL_DECL_VAR; break;
        case TOKEN_FINAL: decl->type = AST_VAL_DECL_FINAL; break;
        case TOKEN_CONST: decl->type = AST_VAL_DECL_CONST; break;
        default: PARSE_ERROR(EXPECTED("final/const/var")); return false;
    }
    Token *name_token = PARSER_EXPECT_NEXT(TOKEN_IDENT, "value decl name");
    decl->name = name_token->ident;
    decl->loc = name_token->location;
    decl->spec_type = NULL;
    if (parser_next_is(parser, TOKEN_COLON)) {
        if (!(decl->spec_type = parse_type(parser))) {
            return false;
        }
    } 
    decl->initializer = NULL;
    if (parser_next_is(parser, TOKEN_ASSIGN)) {
        if (!(decl->initializer = parse_expr(parser, token_stop_semicolon))) {
            return false;
        }
    }
    PARSER_EXPECT_NEXT(TOKEN_SEMICOLON, "semicolon");
    return true;
}
