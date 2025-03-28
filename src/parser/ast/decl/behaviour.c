#include "behaviour.h"
#include "ast/interface/generic.h"
#include "ast/node/decl.h"
#include "ast/node/decl/behaviour.h"
#include "core/not_null.h"
#include "core/slice/slice.h"
#include "core/vec.h"
#include "lexer/token/kind.h"
#include "parser/api.h"
#include "parser/ast/shared/func_info.h"
#include "parser/ast/shared/generics.h"

AstNodeDecl *parse_behaviour_decl(Parser *parser) {
    Token *name_token = PARSER_EXPECT_NEXT(TOKEN_IDENT, "behaviour name");
    FileLocation name_loc = name_token->location;
    Slice name = name_token->ident;
    AstGenerics *generics = parse_generics_optional(parser);
    AstBehaviourRule *rules = vec_new(AstBehaviourRule);
    PARSER_EXPECT_NEXT(TOKEN_OPENING_FIGURE_BRACE, "behaviour body");
    while (parser_next_is_not(parser, TOKEN_CLOSING_FIGURE_BRACE)) {
        Token *token = parser_next(parser);
        FileLocation loc = token->location;
        switch (token->kind) {
            case TOKEN_FUN: {
                AstGenerics *generics = NULL;
                rules = vec_push_dir(rules, ast_behaviour_rule_new_func(loc, NOT_NULL(parse_func_info(parser, &generics))));
                break;
            }
            default:
                PARSE_ERROR(EXPECTED("behaviour rule"));
        }
    }
    return ast_node_decl_new_generic(name_loc, ast_node_decl_generic_new_behaviour(generics,
        ast_behaviour_decl_new(name, name_loc, rules)
    ));
}
