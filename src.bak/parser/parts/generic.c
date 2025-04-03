#include "parser/parts/generic.h"
#include "ast/private/generic.h"
#include "ast/private/path.h"
#include "core/vec.h"
#include "lexer/token.h"
#include "parser/parts/path.h"
#include "parser/private.h"
#include <stdlib.h>
#include <stdbool.h>

bool parser_continue_generic_rules(Parser *parser, AstGenericRules *rules) {
    do {
        AstPath *path = parse_path(parser);
        if (!path) {
            return false;
        }
        rules->behaviours = vec_push(rules->behaviours, &path);
    } while (parser_next_is(parser, TOKEN_ADD));
    return true;
}

AstGeneric *parse_generic(Parser *parser) {
    AstGeneric *result = malloc(sizeof(AstGeneric));
    result->params = vec_new(AstGenericParam);
    while (true) {
        if (parser_next_is(parser, TOKEN_GREATER)) {
            return result;
        }
        AstGenericParam param;
        Token *name_token = PARSER_EXPECT_NEXT(TOKEN_IDENT, "generic name");
        param.name = name_token->ident;
        param.loc = name_token->location;
        param.rules.behaviours = vec_new(AstPath*);
        if (parser_next_is(parser, TOKEN_COLON)) {
            if (!parser_continue_generic_rules(parser, &param.rules)) {
                return false;
            }
        }
        result->params = vec_push(result->params, &param);
        if (parser_next_is(parser, TOKEN_GREATER)) {
            return result;
        }
        PARSER_EXPECT_NEXT(TOKEN_COMMA, "comma");
    }
}
