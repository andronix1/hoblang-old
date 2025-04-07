#include "asm.h"
#include "ast/node/stmt/asm.h"
#include "core/not_null.h"
#include "core/vec.h"
#include "lexer/token/kind.h"
#include "parser/api.h"
#include "parser/ast/shared/expr.h"

AstInlineAsm *parse_inline_asm(Parser *parser) {
    AstClobber *clobbers = vec_new(AstClobber);
    if (parser_next_is(parser, TOKEN_OPENING_CIRCLE_BRACE)) {
        while (parser_next_is_not(parser, TOKEN_CLOSING_CIRCLE_BRACE)) {
            Token *clobber_token = PARSER_EXPECT_NEXT(TOKEN_IDENT, "register name");
            clobbers = vec_push_dir(clobbers, ast_inline_asm_clobber_new(clobber_token->ident, clobber_token->location));
            switch (parser_next(parser)->kind) {
                case TOKEN_CLOSING_CIRCLE_BRACE:
                    parser_skip_next(parser);
                    break;
                case TOKEN_COMMA:
                    break;
                default:
                    PARSE_ERROR(EXPECTED("comma or )"));
                    return NULL;
            }
        }
    }
    AstInlineAsmFlags flags = 0;
    while (parser_next_is_not(parser, TOKEN_OPENING_FIGURE_BRACE)) {
        switch (parser_next(parser)->kind) {
            case TOKEN_VOLATILE:
                flags |= AST_INLINE_ASM_VOLATILE;
                break;
            default:
                PARSE_ERROR(EXPECTED("asm inline flag"));
                return NULL;
        }
    }
    AstInlineAsmMnemonic *mnemonics = vec_new(AstInlineAsmMnemonic);
    while (parser_next_is_not(parser, TOKEN_CLOSING_FIGURE_BRACE)) {
        Slice name = PARSER_EXPECT_NEXT(TOKEN_IDENT, "mnemonic name")->ident;
        AstInlineAsmArg *args = vec_new(AstInlineAsmArg);
        while (parser_next_is_not(parser, TOKEN_SEMICOLON)) {
            Token *token = parser_next(parser);
            FileLocation loc = token->location;
            switch (token->kind) {
                case TOKEN_DOLLAR:
                    args = vec_push_dir(args, ast_inline_asm_arg_new_reg(loc, PARSER_EXPECT_NEXT(TOKEN_IDENT, "register name")->ident));
                    break;
                case TOKEN_OPENING_SQUARE_BRACE:
                    args = vec_push_dir(args, ast_inline_asm_arg_new_address(loc, NOT_NULL(parse_expr(parser))));
                    PARSER_EXPECT_NEXT(TOKEN_CLOSING_SQUARE_BRACE, "]");
                    break;
                default:
                    parser_skip_next(parser);
                    args = vec_push_dir(args, ast_inline_asm_arg_new_expr(loc, NOT_NULL(parse_expr(parser))));
                    break;
            }
            switch (parser_next(parser)->kind) {
                case TOKEN_SEMICOLON:
                    parser_skip_next(parser);
                    break;
                case TOKEN_COMMA:
                    break;
                default:
                    PARSE_ERROR(EXPECTED("comma or `;`"));
                    return NULL;
            }
        }
        mnemonics = vec_push_dir(mnemonics, ast_inline_asm_mnemonic_new(name, args));
    }
    return ast_inline_asm_new(flags, clobbers, mnemonics);
}
