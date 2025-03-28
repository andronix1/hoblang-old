#include "val_info.h"
#include "ast/shared/val_info.h"
#include "core/location.h"
#include "core/not_null.h"
#include "lexer/token/kind.h"
#include "parser/api.h"
#include "parser/ast/shared/type.h"

AstValInfo *parse_val_info(Parser *parser) {
    AstValInfoKind kind;
    switch (parser_next(parser)->kind) {
        case TOKEN_VAR: kind = AST_VAL_INFO_VAR; break;
        case TOKEN_FINAL: kind = AST_VAL_INFO_FINAL; break;
        case TOKEN_CONST: kind = AST_VAL_INFO_CONST; break;
        default:
            PARSE_ERROR(EXPECTED("var, final or const"));
            return NULL;
    }
    Token *name_token = PARSER_EXPECT_NEXT(TOKEN_IDENT, "value's name");
    Slice name = name_token->ident;
    FileLocation name_loc = name_token->location;
    AstType *type = NULL;
    if (parser_next_is(parser, TOKEN_COLON)) {
        type = NOT_NULL(parse_type(parser));
    }
    return ast_val_info_new(kind, name, name_loc, type);
}



