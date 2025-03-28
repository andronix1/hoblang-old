#include "func_info.h"
#include "ast/interface/type.h"
#include "ast/shared/generic.h"
#include "ast/shared/func_info.h"
#include "core/location.h"
#include "core/not_null.h"
#include "core/vec.h"
#include "lexer/token/kind.h"
#include "parser/api.h"
#include "parser/ast/shared/generics.h"
#include "parser/ast/shared/type.h"

AstFuncInfo *parse_func_info(Parser *parser, AstGenerics **generics_output) {
    AstType *ext_of = NULL;
    if (parser_next_is(parser, TOKEN_OPENING_CIRCLE_BRACE)) {
        ext_of = NOT_NULL(parse_type(parser));
        PARSER_EXPECT_NEXT(TOKEN_CLOSING_CIRCLE_BRACE, "`)`");
    }
    Token *name_token = PARSER_EXPECT_NEXT(TOKEN_IDENT, "function name");
    FileLocation name_loc = name_token->location;
    Slice name = name_token->ident;
    AstGenerics *generics = parse_generics_optional(parser);
    if (generics_output) {
        *generics_output = generics;
    } else if (generics) {
        PARSE_ERROR("generics are not allowed here");
    }
    PARSER_EXPECT_NEXT(TOKEN_OPENING_CIRCLE_BRACE, "`(`");
    AstFuncInfoArg *args = vec_new(AstFuncInfoArg);
    while (parser_next_is_not(parser, TOKEN_CLOSING_CIRCLE_BRACE)) {
        Token *name_token = PARSER_EXPECT_NEXT(TOKEN_IDENT, "function name");
        FileLocation name_loc = name_token->location;
        Slice name = name_token->ident;
        PARSER_EXPECT_NEXT(TOKEN_COLON, "`:`");
        AstType *type = NOT_NULL(parse_type(parser));
        args = vec_push_dir(args, ast_func_info_arg_new(name, name_loc, type));
        switch (parser_next(parser)->kind) {
            case TOKEN_COMMA:
                break;
            case TOKEN_CLOSING_CIRCLE_BRACE:
                parser_skip_next(parser);
                break;
            default:
                PARSE_ERROR(EXPECTED("comma or `)`"));
                break;
        }
    }
    PARSER_EXPECT_NEXT(TOKEN_FUNC_RETURNS, "`->`");
    AstType *returns = NOT_NULL(parse_type(parser));
    return ast_func_info_new(name, name_loc, args, ext_of, returns);
}

