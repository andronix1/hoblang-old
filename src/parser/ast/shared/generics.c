#include "generics.h"
#include "ast/interface/behaviour.h"
#include "ast/shared/generic.h"
#include "core/vec.h"
#include "lexer/token/kind.h"
#include "parser/api.h"
#include "parser/ast/shared/behaviour.h"

AstGenerics *parse_generics(Parser *parser) {
    AstGenericParam *params = vec_new(AstGenericParam);
    while (parser_next_is_not(parser, TOKEN_GREATER)) {
        Token *name_token = PARSER_EXPECT_NEXT(TOKEN_IDENT, "generic name");
        Slice name = name_token->ident;
        FileLocation name_loc = name_token->location;
        AstBehaviour *behaviour = parser_next_is(parser, TOKEN_COLON) ?
            parse_behaviour(parser) : NULL;
        params = vec_push_dir(params, ast_generic_param_new(name, name_loc, behaviour));
        switch (parser_next(parser)->kind) {
            case TOKEN_COMMA: break;
            case TOKEN_GREATER:
                parser_skip_next(parser);
                break;
            default:
                PARSE_ERROR(EXPECTED("comma or `>`"));
                break;
        }
    }
    return ast_generics_new(params);
}

AstGenerics *parse_generics_optional(Parser *parser) {
    return parser_next_is(parser, TOKEN_LESS) ? parse_generics(parser) : NULL;
}



