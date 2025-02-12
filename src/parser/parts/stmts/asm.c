#include "../../parts.h"
#include "../expr.h"
#include "ast/private/stmts/asm.h"

bool token_is_asm_arg_end(TokenType type) {
    return type == TOKEN_COMMA || type == TOKEN_SEMICOLON;
}

bool token_is_asm_address_end(TokenType type) {
    return type == TOKEN_CLOSING_SQUARE_BRACE;
}

bool parse_asm_mnemonic(Parser *parser, AstAsmMnemonic *mnem) {
    mnem->name = PARSER_EXPECT_NEXT(TOKEN_IDENT, "asm mnemonic")->ident;
    mnem->args = vec_new(AstAsmArg);
    if (parser_next_is(parser, TOKEN_SEMICOLON)) {
        return true;
    }
    while (true) {
        switch (parser_next(parser)->type) {
            case TOKEN_DOLLAR: {
                AstAsmArg arg = {
                    .type = AST_ASM_ARG_REGISTER,
                    .reg = PARSER_EXPECT_NEXT(TOKEN_IDENT, "register name")->ident
                };
                mnem->args = vec_push(mnem->args, &arg);
                break;
            }

            case TOKEN_OPENING_SQUARE_BRACE: {
                AstAsmArg arg = { .type = AST_ASM_ARG_ADDRESS };
                if (!(arg.expr = parse_expr(parser, token_is_asm_address_end))) {
                    return false;
                }
                PARSER_EXPECT_NEXT(TOKEN_CLOSING_SQUARE_BRACE, "address close");
                mnem->args = vec_push(mnem->args, &arg);
                break;
            }

            default: {
                parser_skip_next(parser);
                AstAsmArg arg = {
                    .type = AST_ASM_ARG_EXPR
                };
                if (!(arg.expr = parse_expr(parser, token_is_asm_arg_end))) {
                    return false;
                }
                parser_skip_next(parser);
                mnem->args = vec_push(mnem->args, &arg);
                break;
            }
        }
        switch (parser_next(parser)->type) {
            case TOKEN_COMMA:
                break;
            case TOKEN_SEMICOLON:
                return true;
            default:
                PARSE_ERROR(EXPECTED("asm mnemonic arg break"));
                return false;
        }
    }
}

bool parse_asm_body(Parser *parser, AstInlineAsm *inline_asm) {
    parser_step(parser);
    inline_asm->clobbers = vec_new(Slice);
    if (parser_token(parser)->type == TOKEN_OPENING_CIRCLE_BRACE) {
        parser_step(parser);
        if (parser_token(parser)->type != TOKEN_CLOSING_CIRCLE_BRACE) {
            bool reading = true;
            parser_skip_next(parser);
            while (reading) {
                inline_asm->clobbers = vec_push(
                    inline_asm->clobbers,
                    &PARSER_EXPECT_NEXT(TOKEN_IDENT, "register name")->ident
                );
                switch (parser_next(parser)->type) {
                    case TOKEN_CLOSING_CIRCLE_BRACE:
                        reading = false;
                        break;
                    case TOKEN_COMMA:
                        break;
                    default:
                        PARSE_ERROR("expected clobber break");
                        return false;
                }
            }
        }
    } else {
        parser_skip_next(parser);
    }
    inline_asm->is_volatile = parser_next_is(parser, TOKEN_VOLATILE);
    PARSER_EXPECT_NEXT(TOKEN_OPENING_FIGURE_BRACE, "asm body opening");
    inline_asm->mnems = vec_new(AstAsmMnemonic);
    while (true) {
        parser_step(parser);
        if (parser_token(parser)->type == TOKEN_CLOSING_FIGURE_BRACE) {
            return true;
        }
        parser_skip_next(parser);
        AstAsmMnemonic mnem;
        if (!parse_asm_mnemonic(parser, &mnem)) {
            return false;
        }
        inline_asm->mnems = vec_push(inline_asm->mnems, &mnem);
    }
}

