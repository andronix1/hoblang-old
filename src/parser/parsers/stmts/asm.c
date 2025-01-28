#include "../../parsers.h"

bool token_is_asm_arg_end(TokenType type) {
    return type == TOKEN_COMMA || type == TOKEN_SEMICOLON;
}

bool parse_asm_mnemonic(Parser *parser, AstAsmMnemonic *mnem) {
    parse_exp_next(TOKEN_IDENT, "asm mnemonic");
    mnem->name = parser->token->ident;
    mnem->args = vec_new(AstAsmArg);
    parser_next_token(parser);
    if (token_type(parser->token) == TOKEN_SEMICOLON) {
        return true;
    }
    parser->skip_next = true;
    while (true) {
        parser_next_token(parser);
        switch (token_type(parser->token)) {
            case TOKEN_DOLLAR: {
                parse_exp_next(TOKEN_IDENT, "register name");
                AstAsmArg arg = {
                    .type = AST_ASM_ARG_REGISTER,
                    .reg = parser->token->ident
                };
                mnem->args = vec_push(mnem->args, &arg);
                break;
            }

            case TOKEN_OPENING_SQUARE_BRACE: {
                AstAsmArg arg = { .type = AST_ASM_ARG_VALUE };
                if (!parse_value(parser, &arg.value)) {
                    return false;
                }
                parse_exp_next(TOKEN_CLOSING_SQUARE_BRACE, "address close");
                mnem->args = vec_push(mnem->args, &arg);
                break;
            }

            default: {
                parser->skip_next = true;
                AstAsmArg arg = {
                    .type = AST_ASM_ARG_EXPR
                };
                if (!(arg.expr = parse_expr_before(parser, token_is_asm_arg_end))) {
                    return false;
                }
                parser->skip_next = true;
                mnem->args = vec_push(mnem->args, &arg);
                break;
            }

            // default:
            //     parse_err(EXPECTED("asm arg"));
            //     break;
        }
        parser_next_token(parser);
        switch (token_type(parser->token)) {
            case TOKEN_COMMA:
                break;
            case TOKEN_SEMICOLON:
                return true;
            default:
                parse_err(EXPECTED("asm mnemonic arg break"));
                return false;
        }
    }
}

bool parse_asm_body(Parser *parser, AstInlineAsm *inline_asm) {
    parse_exp_next(TOKEN_OPENING_CIRCLE_BRACE, "asm clobers opening");
    inline_asm->clobbers = vec_new(Slice);
    bool reading = true;
    parser_next_token(parser);
    if (token_type(parser->token) == TOKEN_CLOSING_CIRCLE_BRACE) {
        return true;
    }
    parser->skip_next = true;
    while (reading) {
        parse_exp_next(TOKEN_IDENT, "register name");
        inline_asm->clobbers = vec_push(inline_asm->clobbers, &parser->token->ident);
        parser_next_token(parser);
        switch (token_type(parser->token)) {
            case TOKEN_CLOSING_CIRCLE_BRACE:
                reading = false;
                break;
            case TOKEN_COMMA:
                break;
            default:
                parse_err("expected clobber break");
                return false;
        }
    }
    parser_next_token(parser);
    if (token_type(parser->token) == TOKEN_VOLATILE) {
        inline_asm->is_volatile = true;
        parser_next_token(parser);
    } else {
        inline_asm->is_volatile = false;
    }
    parse_exp(TOKEN_OPENING_FIGURE_BRACE, "asm body opening");
    inline_asm->mnems = vec_new(AstAsmMnemonic);
    while (true) {
        parser_next_token(parser);
        if (token_type(parser->token) == TOKEN_CLOSING_FIGURE_BRACE) {
            return true;
        }
        parser->skip_next = true;
        AstAsmMnemonic mnem;
        if (!parse_asm_mnemonic(parser, &mnem)) {
            return false;
        }
        inline_asm->mnems = vec_push(inline_asm->mnems, &mnem);
    }
}

