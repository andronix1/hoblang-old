#include "ast/private/decls/behaviour.h"
#include "core/vec.h"
#include "lexer/token.h"
#include "parser/parser.h"
#include "parser/parts/func_info.h"
#include "parser/parts/type.h"
#include "parser/private.h"

bool parse_behaviour_decl(Parser *parser, AstDeclBehaviour *decl) {
    decl->name = PARSER_EXPECT_NEXT(TOKEN_IDENT, "behaviour name")->ident;
    PARSER_EXPECT_NEXT(TOKEN_OPENING_FIGURE_BRACE, "behaviour body");
    decl->rules = vec_new(AstBehaviourRule);
    while (true) {
        AstBehaviourRule rule;
        if (parser_next_is(parser, TOKEN_CLOSING_FIGURE_BRACE)) {
            return true;
        }
        PARSER_EXPECT_NEXT(TOKEN_FUN, "fun");
        rule.type = AST_BEHAVIOUR_RULE_FUNC;
        rule.func.in_type = NULL;
        if (parser_next_is(parser, TOKEN_OPENING_CIRCLE_BRACE)) {
            if (!(rule.func.in_type = parse_type(parser))) {
                return false;
            }
        }
        PARSER_EXPECT_NEXT(TOKEN_CLOSING_CIRCLE_BRACE, "closing circle brace"); 
        rule.func.name = PARSER_EXPECT_NEXT(TOKEN_IDENT, "func name")->ident;
        if (!parse_func_type_info(parser, &rule.func.info)) {
            return false;
        }
        decl->rules = vec_push(decl->rules, &rule);
    }
}

